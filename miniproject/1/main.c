/*
	strtok_r for thread safety
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "miniproject.h"
#include "circular_buffer.h"

const float Period = 0.1;

const int Reference = 1;
const int Kp = 10;
const int Ki = 800;


float y;

struct udp_conn conn;
struct timespec timeSpec;

static void *network_sender(void *var)  {
	while (1) {
		// udp_receive
		// put into correct queue
	}
}

static void *network_receiver(void *var) {

}

static void *controller(void *var)  {
	
}
static void *signal_handler(void *var)  {
	return;
}

float controller(y) {
	static float integral = 0;

	float error = Reference - y;
	integral = integral + (error * Period);
	return ((float)Kp*error + (float)Ki*integral);
}

void e_udp_send(char* str, int len) {
	if (udp_send(&conn, str, len) == -1)
		printf("failed to send\n");
}

void e_udp_receive(char *buf, int len, char c) {
	if (udp_receive(&conn, buf, len) == -1)
		printf("failed to receive\n");
}

float getY() {
	char requestMessage[] = "GET";
	e_udp_send(requestMessage, sizeof(requestMessage));
	char receiveBuffer[255];
	e_udp_receive(receiveBuffer, 255);
	float y;
	y = atof(strtok(string, ":"));
	return y; 
}

// GET-MESSAGE:1.223

void setU(float y) {
	char buf[255], buf2[255]

	u = controller(y);
	snprintf(buf2, 255, "%f", u);
	strcpy(buf, "SET:");
	strcat(buf, buf2);
	printf("client sends: %s\n", buf);
	e_udp_send(buf, strlen(buf));

}

// HUSK AT SERVER SENDER ANDRE MELDINGER OGSÅ
float getValFromMessage(char* msg) {
	int i = 0;
	while (msg[i] != ':')
		i++;
	int j = ++i;
	char val[255];
	int k = 0;
	while (msg[j] != '\0') {
		val[k] = msg[j];
		j++;
		k++;
	}
	val[++k] = '\0';
	return atof(val);
}

void configureSleepTimer(long us) {
	clock_gettime(CLOCK_REALTIME, &timeSpec);
	timespec_add_us(&timeSpec, us);
}

int main() {
	printf("Starting!\n");
	start_server();
	char hostAddress[] = "192.168.0.1";
	if (udp_init_client(&conn, 9999, hostAddress) != 0)
		printf("failed to init\n");
	// start simulator
	e_udp_send("START", strlen("START"));
	
	// find better solution for receive buffer
	char buf[255], buf2[255], str[255];
	float y, u;
	while (1) {
		printf("e_udp_send()\n");
		e_udp_send("GET", strlen("GET"));
		e_udp_receive(str, 255, G);
		while(str[0] != 'G') {
			printf("e_udp_receive()\n");
			e_udp_receive(str, 255);
		}
		y = getValFromMessage(str);
		printf("server sends: %f\n", y);
		u = controller(y);
		printf("u: %f\n", u);


		snprintf(buf2, 255, "%f", u);
		strcpy(buf, "SET:");
		strcat(buf, buf2);
		printf("client sends: %s\n", buf);
		e_udp_send(buf, strlen(buf));
		// if 0.5s, send: STOP
		configureSleepTimer(1);
		clock_nanosleep(&timeSpec);
		// break;
	}
	stop_server();
	udp_close(&conn);
	return 0;
}


