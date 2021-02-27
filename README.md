<p align="center">
    <img src="https://arquivo.devmedia.com.br/artigos/Ricardo_Teixeira/Introducao_MySQL/Introducao_MySQL1.jpg">
</p>
   

# MySQL
## Introdução
MySQL é um sistema de gerenciamento de banco de dados... Para saber mais clique [aqui](https://pt.wikipedia.org/wiki/MySQL)
### Instalação
```bash
$ ssh pi@<ip>
$ sudo apt-get update
$ sudo apt-get install mysql-server
```

As vezes o mysql não configura a senha default, para solucionar esse problema clique nesse [link](https://www.a2hosting.com.br/kb/developer-corner/mysql/reset-mysql-root-password).

### Criação da base de dados 
Para exemplificar será guardado o [epoch](https://www.unixtutorial.org/unix-epoch/) para a alteração do LED mediante ao pressionamento da tecla.

Primeiro é necessário criar a base de dados, o nome da base de dados vai se chamar de **led_info**. Para criar essa base de dados acesse o mysql:

```bash
$ mysql -u <user> -p
```

O prompt será apresentado com o simbolo de maior **>**. Para a criação da base de dados execute o seguinte comando:

```bash
> create database led_info;
```

_output_
```bash
Query OK, 1 row affected (0.00 sec)
```

Para verificar se a base foi criada execute o comando:

```bash
> show databases;
```

A base será listada

_output_
```bash
+--------------------+
| Database           |
+--------------------+
| information_schema |
| led_info           |
| mysql              |
| performance_schema |
+--------------------+
4 rows in set (0.00 sec)
```

Para começar a usar a base de dados execute o comando:

```sql
> use led_info;
```

_output_
```
Database changed
```

Com a base criada é chegada a hora de criar a tabela que irá de fato guardar as informações sobre os eventos 
criados pela a alteração do estado do LED.

### Criação da tabela

A tabela para armazenar o momento que o LED mudou de estado, faz-se necessário criar um campo do tipo numérico, para saber mais clique [aqui](https://www.w3schools.com/sql/sql_datatypes.asp). O esquema da tabela fica dessa forma:

```bash
CREATE TABLE led_table(
    id          int AUTO_INCREMENT,
    timestamp   NUMERIC NOT NULL,
    state       VARCHAR(10) NOT NULL, 
    PRIMARY KEY (id)
);
```
Para criar essa tabela, basta digitar na linha de comando:

```bash 
> create table led_table(id int auto_increment, timestamp numeric not null, state varchar(10) not null, primary key(id));
```

_output_
```bash
Query OK, 0 rows affected (0.06 sec)
```

Uma vez criada é possível consultar se a tabela foi criada.

```bash
> show tables;
```

_output_
```bash
+--------------------+
| Tables_in_led_info |
+--------------------+
| led_table          |
+--------------------+
1 row in set (0.00 sec)

```

Com a tabela criada, agora é possível inserir os dados na tabela.

### Inserindo dados na tabela
Apartir do comando _insert_ é possível inserir dados na tabela, para isso é criada uma _query_ para compor a _string_ que será enviada para o banco de dados.

```bash
> insert into led_info (timestamp, state) values (1609090934, 'ON');
```

_output_
```bash
Query OK, 1 row affected (0.01 sec)
```

A _query_ criada informa o seguinte: Insira na tabela led_info no campo (timestamp) o valor 1609090934, e no campo state a string 'ON'.

### Consultando os dados armazenados na tabela

Através do comando _select_ é possível consultar os dados presente na tabela:

```bash
> select * from led_table;
```

_output_
```bash
+----+------------+-------+
| id | timestamp  | state |
+----+------------+-------+
|  1 | 1609090934 | ON    |
+----+------------+-------+
1 row in set (0.00 sec)
```

## MySQL no Rasbperry Pi
### Instalação do pacote de desenvolvimento
<strong>Obs:</strong> Para a compilação desse projeto é necessário instalar a lib hardware presente [aqui](https://github.com/NakedSolidSnake/Raspberry_lib_hardware).

Para poder usar o MySQL através da linguagem C é necessário instalar a lib de desenvolvimento do MySQL

```bash
$ sudo apt-get install default-libmysqlclient-dev
```

Após o término da instalação, é possível compilar o projeto.

## Implementação
```c
#include <button.h>
#include <led.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <json.h>
#include <db.h>
#include <time.h>

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
char *insert(void *data);

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

    initdb(database.hostname, database.username, database.password, database.database);

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

            insertdb(insert, &eState);
            state = 0;
        }

        usleep(_1MS);
    }

    closedb();
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

char *insert(void *data)
{
    static char query[1024] = {0};
    int state = *(int *)data;
    time_t epoch;

    epoch = time(NULL);

    snprintf(query, 1024, "insert into led_table (timestamp, state) values (%ld, '%s')", epoch, state ? "OFF": "ON");

    return query;
}
```

## Conclusão
