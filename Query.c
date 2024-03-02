

#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>
//FunciÛn que devuelve la partida con menos preguntas contestadas correctamente entre todos los jugadores y el usuario que ganÛ esa Partida




// Estructura para representar una partida
typedef struct {
	int id_partida;
	int id_usuario;
	int preguntas_correctas;
} Partida;
// Estructura para representar un jugador
typedef struct {
	int id_usuario;
	char nombre_usuario[100]; // Suponemos que el nombre de usuario puede tener un m√°ximo de 100 caracteres
} Jugador;
// Funci√≥n para encontrar la partida con menos preguntas correctas y su ganador
void encontrarPartidaMenosCorrectas(MYSQL *conn) {
	// Consulta SQL para obtener la partida con menos preguntas correctas
	char query[200];
	sprintf(query, "SELECT * FROM partida ORDER BY numero_preguntas_correctas ASC LIMIT 1");
	
	// Ejecutar la consulta
	if (mysql_query(conn, query)) {
		fprintf(stderr, "Error al ejecutar la consulta: %s\n", mysql_error(conn));
		return;
	}
	MYSQL_RES *result = mysql_store_result(conn);
	if (result == NULL) {
		fprintf(stderr, "Error al obtener el resultado: %s\n", mysql_error(conn));
		return;
	}
	// Obtener la fila de la consulta
	MYSQL_ROW row;
	if ((row = mysql_fetch_row(result))) {
		Partida partida;
		partida.id_partida = atoi(row[0]);
		partida.id_usuario = atoi(row[1]);
		partida.preguntas_correctas = atoi(row[4]);
		// Consulta para obtener el nombre del ganador
		sprintf(query, "SELECT nombre_usuario FROM jugadores WHERE id_usuario = %d", partida.id_usuario);
		if (mysql_query(conn, query)) {
			fprintf(stderr, "Error al ejecutar la consulta: %s\n", mysql_error(conn));
			return;
		}
		MYSQL_RES *result2 = mysql_store_result(conn);
		if (result2 == NULL) {
			fprintf(stderr, "Error al obtener el resultado: %s\n", mysql_error(conn));
			return;
		}
		MYSQL_ROW row2;
		if ((row2 = mysql_fetch_row(result2))) {
			printf("Partida con menos preguntas correctas:\n");
			printf("ID Partida: %d\n", partida.id_partida);
			printf("ID Usuario Ganador: %d\n", partida.id_usuario);
			printf("Nombre Usuario Ganador: %s\n", row2[0]);
			printf("Preguntas Correctas: %d\n", partida.preguntas_correctas);
		}
		mysql_free_result(result2);
	} else {
		printf("No se encontraron partidas.\n");
	}
	
	mysql_free_result(result);
}	
int main() {
	MYSQL *conn = mysql_init(NULL);
	if (conn == NULL) {
		fprintf(stderr, "Error al inicializar la conexi√≥n: %s\n", mysql_error(conn));
		return 1;
	}
	
	if (mysql_real_connect(conn, "localhost", "root", "mysql", "Kahoot", 0, NULL, 0) == NULL) {
		fprintf(stderr, "Error al conectarse a la base de datos: %s\n", mysql_error(conn));
		mysql_close(conn);
		return 1;
	}		
	encontrarPartidaMenosCorrectas(conn);
	
	mysql_close(conn);
	return 0;	

encontrarPartidaMenosCorrectas(conn);

mysql_close(conn);
return 0;
}
			
