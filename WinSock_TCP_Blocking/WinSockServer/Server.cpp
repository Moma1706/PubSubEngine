#include "../Libraries/ServerFuncionality.h"

int  main(void)
{

	DWORD PubThreadId, SubThreadId;
	HANDLE SubThread, PubThread;

	ht_t* publisher_map = ht_create();
	ht_s_t* subscriber_map = ht_create_sub();

	SUB_PARAMS sub_params;
	PUB_PARAMS pub_params;

	sub_params.publisher_map = publisher_map;
	sub_params.subscriber_map = subscriber_map;
	sub_params.port = DEFAULT_SUB_PORT;

	pub_params.publisher_map = publisher_map;
	pub_params.subscriber_map = subscriber_map;
	pub_params.port = DEFAULT_PUB_PORT;
	
	PubThread = CreateThread(NULL, 0, &thread_function, &pub_params, 0, &PubThreadId);

	SubThread = CreateThread(NULL, 0, &thread_function, &sub_params, 0, &SubThreadId);
	
	getchar();

	return 0;
}



