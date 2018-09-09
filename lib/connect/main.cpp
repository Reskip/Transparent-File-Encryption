#include "connect.h"

#include <cstdio>
int main()
{
	int level = 0;
	int status = 0;

	Connect client = Connect();
	status = client.login("admin", "555", level);
	printf("%d\n%d\n", status, level);
	return 0;
}