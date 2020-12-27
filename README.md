![mysql](https://arquivo.devmedia.com.br/artigos/Ricardo_Teixeira/Introducao_MySQL/Introducao_MySQL1.jpg)

# MySQL
## Introdução
## MySQL 
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
    PRIMARY KEY (id)
);
```
Para criar essa tabela, basta digitar na linha de comando:

```bash 
> create table led_table(id int auto_increment, timestamp numeric not null, primary key(id));
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
> insert into led_info (timestamp) values (1609072545);
```

_output_
```bash
Query OK, 1 row affected (0.01 sec)
```

A _query_ criada informa o seguinte: Insira na tabela led_info no campo (timestamp) o valor 1609072545.

### Consultando os dados armazenados na tabela

Através do comando _select_ é possível consultar os dados presente na tabela:

```bash
> select * from led_table;
```

_output_
```bash
+----+------------+
| id | timestamp  |
+----+------------+
|  1 | 1609072545 |
+----+------------+
1 row in set (0.00 sec)
```

## Conclusão