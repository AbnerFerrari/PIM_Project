#include <stdio.h>
#include <stdlib.h>
#include "../shared/client.h"

int main(){
	const int LOGIN_STR_LENGTH = 80;
	const int PASSWORD_STR_LENGTH = 50;
	char login[LOGIN_STR_LENGTH];
	char password[PASSWORD_STR_LENGTH];
	
	printf("Login (%d caracteres): ", LOGIN_STR_LENGTH);
	scanf("%s", &login);

	printf("Senha (%d caracteres): ", PASSWORD_STR_LENGTH);
	scanf("%s", &password);

	log_in(login, password);
	
	return 0;
}
