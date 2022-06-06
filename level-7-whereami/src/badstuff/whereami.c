/*

- Where am I? Silicon 2022
- Author: DS-Koolaid

apt-get install gcc
apt-get install libcurl4-openssl-dev

 gcc -Wall whereami.c -lcurl -o whereami

*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <sys/stat.h>

size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp);


void XOR(char * data, size_t data_len, char * key, size_t key_len) {
	int j;
	j = 0;
	for (int i = 0; i < data_len; i++) {
		if (j == key_len - 1) j = 0;

		data[i] = data[i] ^ key[j];
		j++;
	}
	
}

int main(void) {
	
	char key[]="91HcwAhvxh";
	unsigned char flag[] = { 0x6a, 0x78, 0x4, 0x2a, 0x34, 0xe, 0x26, 0xd, 0x39, 0x1e, 0x9, 0x58, 0x2c, 0x54, 0x19, 0x26, 0x37, 0x52, 0x19, 0x6, 0x5d, 0x9, 0x27, 0x1b, 0x44, 0x32, 0x37, 0x30, 0x48, 0x1a, 0x66, 0x65, 0x20, 0x6, 0x28, 0x16, 0x1, 0x18, 0x5, 0x68 };
	unsigned char resolve[] = { 0x4e, 0x50, 0x26, 0xd, 0x16, 0x2c, 0x9, 0x1b, 0x15, 0x9, 0x54, 0x58, 0x29, 0x4d, 0x15, 0x2e, 0x1f, 0x5, 0x1d, 0x1a, 0x5a, 0x5e, 0x3a, 0x13, 0x59, 0x2d, 0x7, 0x15, 0x19, 0x4, 0x39 };
	unsigned char tmp_hosts_check_command[] = { 0x16, 0x59, 0x27, 0xe, 0x12, 0x6e, 0x1b, 0x1e, 0x1, 0xf, 0x4c, 0x48, 0x67, 0x4d, 0x3, 0x2c, 0x18, 0x29, 0x10, 0x7, 0x4a, 0x45, 0x3b, 0x3c, 0x14, 0x29, 0xd, 0x15, 0x13, 0xd, 0x4b, 0x31 };
	unsigned char resolve_host_setting_string[] = { 0x4e, 0x50, 0x26, 0xd, 0x16, 0x2c, 0x9, 0x1b, 0x15, 0x9, 0x54, 0x58, 0x29, 0x4d, 0x15, 0x2e, 0x1f, 0x5, 0x1d, 0x1a, 0x5a, 0x5e, 0x3a, 0x13, 0x59, 0x2d, 0x7, 0x15, 0x19, 0x4, 0x3, 0x9, 0x78, 0x59, 0x46, 0x73, 0x5f, 0x58, 0x48, 0x46, 0x9, 0x1f, 0x79, 0x63 };
	unsigned char payload_dump_command[] = { 0x16, 0x53, 0x21, 0xd, 0x58, 0x2c, 0x1e, 0x56, 0x57, 0x1a, 0x56, 0x5e, 0x3c, 0x4c, 0x0, 0x20, 0x6, 0x18, 0x19, 0x5, 0x58, 0x5c, 0x25, 0x2, 0x1a, 0x28, 0x9, 0x58, 0xc, 0x9, 0x4b, 0x11, 0x67, 0x17, 0x1a, 0x31, 0x47, 0x1, 0x19, 0x6, 0x57, 0x50, 0x25, 0xe, 0x16, 0x2c, 0x1, 0x17, 0x56, 0x1c, 0x58, 0x43, 0x68, 0x45, 0x51, 0x61, 0xb, 0x1e, 0x15, 0x7, 0x5d, 0x11, 0x7f, 0x54, 0x40, 0x61, 0x47, 0x2, 0x15, 0x18, 0x16, 0x46, 0x29, 0xd, 0x19, 0x20, 0x5, 0x1b, 0x19, 0x5, 0x50, 0x50, 0x66, 0x17, 0x16, 0x33, 0x68 };


	int DEBUG_MODE = 0;
	char* debug_envvar = "DEBUG";
	if(getenv(debug_envvar)) {
		DEBUG_MODE = 1;
	}


	int status_code = 0;
	XOR((char *) tmp_hosts_check_command, sizeof(tmp_hosts_check_command), key, sizeof(key));

	status_code = WEXITSTATUS(system(tmp_hosts_check_command));
	if (status_code == 1) {
		if (DEBUG_MODE) {
			printf("/tmp/hosts file not found. Defaulting to /etc/hosts\n");
		}
	}
	else if (status_code == 2) {
		if (DEBUG_MODE) {
			printf("Invalid /tmp/hosts file. Defaulting to /etc/hosts\n");
		}
	}
	else if (status_code == 3) {
		if (DEBUG_MODE) {
			printf("Not what I was expecting...\n");
		}
	}
	
	CURL *curl;
	CURLcode res;
 	curl = curl_easy_init();
  	if(curl) {
  		XOR((char *) resolve, sizeof(resolve), key, sizeof(key));
		struct curl_slist *resolve_host_setting = NULL;

		if (status_code == 0) {
			XOR((char *) resolve_host_setting_string, sizeof(resolve_host_setting_string), key, sizeof(key));
			resolve_host_setting = curl_slist_append(NULL, resolve_host_setting_string);
		}

		curl_easy_setopt(curl, CURLOPT_RESOLVE, resolve_host_setting);
		curl_easy_setopt(curl, CURLOPT_URL, resolve);
    	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);

    	res = curl_easy_perform(curl);

    	long http_code = 0;
		curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &http_code);

    	curl_easy_cleanup(curl);
    	if (http_code == 200){
    		XOR((char *) flag, sizeof(flag), key, sizeof(key));
    		XOR((char *) payload_dump_command, sizeof(payload_dump_command), key, sizeof(key));
			system(payload_dump_command);
    		printf("Sweet! Our malware is in the right place.\n%s\nPayload dropped in /tmp/wannamammamia.tar\n",flag);
    	}
    	else{
    		printf("How did I end up here?!\n");
    	}
    }
    return 0;
}


size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp)
{
   return size * nmemb;
}
