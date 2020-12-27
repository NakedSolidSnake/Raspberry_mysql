#include <button.h>
#include <led.h>
#include <unistd.h>
#include <stdlib.h>
#include <json.h>

typedef struct 
{
    char *hostname;
    char *username;
    char *password;
    char *database;
} Database;

#define _1MS    1000

static void inputHandler(void);
static void loadParams(const char *properties, Database *database);
static int state = 0;

static Button_t button = {
        .gpio.pin = 7,
        .gpio.eMode = eModeInput,
        .ePullMode = ePullModePullUp,
        .eIntEdge = eIntEdgeFalling,
        .cb = inputHandler
    };


int main(int argc, char const *argv[])
{   
    Database database = {0};
    eState_t eState = eStateLow;
    LED_t led = {
        .gpio.pin = 0,
        .gpio.eMode = eModeOutput
    };

    loadParams("properties.json", &database);

    /* Initialize Led and Button */
    if(Button_init(&button))
        return EXIT_FAILURE;
    if(LED_init(&led))
        return EXIT_FAILURE;


    while(1)
    {
        if(state)
        {
            LED_set(&led, eState);
            if(eState == eStateLow)
                eState = eStateHigh;
            else
                eState = eStateLow;

            state = 0;
        }

        usleep(_1MS);
    }

    return 0;
}

static void inputHandler(void)
{
    if(!Button_read(&button)){
        usleep(_1MS * 40);
        while(!Button_read(&button));

        state = 1;
    }
}

static void loadParams(const char *properties, Database *database)
{
    char buffer[1024] = {0};
    IHandler iParams[] = 
    {
        {.token = "hostname", .data = &database->hostname, .type = eType_String, .child = NULL},
        {.token = "username", .data = &database->username, .type = eType_String, .child = NULL},
        {.token = "password", .data = &database->password, .type = eType_String, .child = NULL},
        {.token = "database", .data = &database->database, .type = eType_String, .child = NULL}
    };

    IHandler iDatabase[] = 
    {
        {.token = "database", .data = NULL, .type = eType_Object, .child = iParams, .size = getItems(iParams)}
    };

    if(!getJsonFromFile(properties, buffer, 1024)){
        exit(EXIT_FAILURE);
    }

    processJson(buffer, iDatabase, getItems(iDatabase));
}