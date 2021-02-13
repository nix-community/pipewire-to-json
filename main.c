#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <json.h>
#include <pipewire/pipewire.h>
#include <spa/utils/json.h>

static int json_to_json(struct spa_json *iter, const char *value, int len, struct json_object *obj, const char *key)
{
	struct spa_json it[1];
	int l, res;
	const char *v;
	struct json_object *child_obj = NULL;
	char child_key[256];

	if (spa_json_is_object(value, len)) {
		child_obj = json_object_new_object();
		spa_json_enter(iter, &it[0]);
		while (spa_json_get_string(&it[0], child_key, sizeof(child_key)-1) > 0) {
			if ((l = spa_json_next(&it[0], &v)) <= 0) {
				break;
			}
			if ((res = json_to_json(&it[0], v, l, child_obj, &child_key[0])) < 0) {
				return res;
			}
		}
	}
	else if (spa_json_is_array(value, len)) {
		child_obj = json_object_new_array();
		spa_json_enter(iter, &it[0]);
		while ((l = spa_json_next(&it[0], &v)) > 0) {
			if ((res = json_to_json(&it[0], v, l, child_obj, NULL)) < 0) {
				return res;
			}
		}
	}
	else if (spa_json_is_float(value, len)) {
		float val = 0.0f;
		spa_json_parse_float(value, len, &val);
		child_obj = json_object_new_double(val);
	}
	else if (spa_json_is_bool(value, len)) {
		bool val = false;
		spa_json_parse_bool(value, len, &val);
		child_obj = json_object_new_boolean(val);
	}
	else if (spa_json_is_null(value, len)) {}
	else {
		char *val = alloca(len+1);
		spa_json_parse_string(value, len, val);
		child_obj = json_object_new_string(val);
	}

	if (json_object_is_type(obj, json_type_array)) {
		json_object_array_add(obj, child_obj);
	} else {
		json_object_object_add(obj, key, child_obj);
	}

	return 0;
}

int main(int argc, char** argv) {
	char *data;
	struct stat sbuf;
	int fd;
	struct spa_json it[3];
        int len;
	struct pw_properties *props = NULL;
	void *pstate = NULL;
	const char *prop_value, *prop_key;
	json_object *json_obj;
	const char* json_value;

	if (argc < 3) {
		fprintf(stderr, "USAGE: %s pipewire-config json-output", argv[0]);
		return 0;
	}

	if ((fd = open(argv[1],  O_CLOEXEC | O_RDONLY)) < 0)  {
		perror("open pipewire-cofnig");
		return 1;
	}

	if (fstat(fd, &sbuf) < 0) {
		perror("fstat pipewire-config");
		close(fd);
		return 1;
	}
	data = mmap(NULL, sbuf.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	close(fd);

	if (data == MAP_FAILED) {
		perror("mmap pipewire-config");
		return 1;
	}
	props = pw_properties_new_string(data);
	if (props == NULL) {
		perror("get properties of pipewire-config");
		munmap(data, sbuf.st_size);
		return 1;
	}
	json_obj = json_object_new_object();
	while ((prop_key = pw_properties_iterate(props, &pstate)) != NULL &&
	       (prop_value = pw_properties_get(props, prop_key)) != NULL) {
		spa_json_init(&it[0], prop_value, sbuf.st_size);
		if ((len = spa_json_next(&it[0], &json_value)) <= 0) {
			perror("top-level value of pipewire-config is not a JSON object");
			return 1;
		}
		json_to_json(&it[0], json_value, len, json_obj, prop_key);
	}
	json_object_to_file_ext(argv[2], json_obj, JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY);

	munmap(data, sbuf.st_size);
}
