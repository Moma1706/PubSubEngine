#include "../Libraries/ServerFuncionality.h"

int  main(void)
{

	DWORD PubThreadId, SubThreadId;
	HANDLE SubThread, PubThread;

	ht_t* publisher_map = ht_create();
	ht_s_t* subscriber_map = ht_create_sub();
	ht_socket_t* socket_map = ht_create_socket();

	SUB_PARAMS sub_params;
	PUB_PARAMS pub_params;

	sub_params.publisher_map = publisher_map;
	sub_params.subscriber_map = subscriber_map;
	sub_params.port = DEFAULT_SUB_PORT;
	sub_params.socket_map = socket_map;

	pub_params.publisher_map = publisher_map;
	pub_params.subscriber_map = subscriber_map;
	pub_params.port = DEFAULT_PUB_PORT;
	pub_params.socket_map = socket_map;
	
	PubThread = CreateThread(NULL, 0, &thread_function, &pub_params, 0, &PubThreadId);

	SubThread = CreateThread(NULL, 0, &thread_function, &sub_params, 0, &SubThreadId);

	char quit[BUFER_SIZE] = "";

	bool temp = 1;

	while (temp) {
		gets_s(quit, BUFER_SIZE);

		if (strcmp(quit, "Q") == 0) {

			ht_delete_map_pub(publisher_map);
			ht_delete_map_socket(socket_map);
			ht_delete_map_sub(subscriber_map);

			CloseHandle(PubThread);
			CloseHandle(SubThread);
			delete_cs();
			
				
			temp = 0;

		}
	}

	return 0;
}



