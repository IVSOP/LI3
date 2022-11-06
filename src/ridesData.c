#include "ridesData.h"

#define SIZE 1000

DATA getRidesData(FILE *ptr) {
	RidesStruct **ridesData = malloc(RIDES_ARR_SIZE*sizeof(RidesStruct *));
	GHashTable *cityTable = g_hash_table_new_full(g_str_hash, g_str_equal, NULL, freeArray); // keys levam malloc do array normal, nao vou dar free aqui
	int i;
	while (fgetc(ptr) != '\n'); // avançar a primeira linha (tbm podia ser um seek hardcoded)
	for (i = 0; i < RIDES_ARR_SIZE; i++) ridesData[i] = getRides(ptr, cityTable);

	RidesData *data = malloc(sizeof(RidesData));
	data->ridesArray = ridesData;
	data->cityTable = cityTable;

	// debug preguiçoso, confusao com os tipos
	// GArray *array = g_hash_table_lookup(cityTable, "Braga");
	// RidesStruct *ride = g_array_index(array, RidesStruct *, 0);
	// printf("%d %s %s %s\n", array->len, ride->date, ride->city, ride->user);

	// debug para ver se ficou com todas as cidades
	// guint len;
	// const gchar **arr = (const gchar **)g_hash_table_get_keys_as_array(cityTable, &len);
	// for (i = 0; i < len; i++) {
	// 	printf("%s\n", arr[i]);
	// }
	// free(arr); // isto e preciso?

	return data;
}

RidesStruct * getRides(FILE *ptr, GHashTable *cityTable) {
	int i, count, chr;
	char tempBuffer[16], *city;
	GArray *array;
	RidesStruct *ridesStructArray = malloc(SIZE*sizeof(RidesStruct)), *temp;
	for (i = count = 0; i < SIZE; i++, count++) {
		while ((chr = fgetc(ptr)) != ';');// && chr != -1); // skip id
		ridesStructArray[i].date = loadString(ptr);
		writeString(ptr, tempBuffer);
		ridesStructArray[i].driver = (short) atoi(tempBuffer);
		ridesStructArray[i].user = loadString(ptr);
		city = loadString(ptr);
		ridesStructArray[i].city = city;
		writeString(ptr, tempBuffer);
		ridesStructArray[i].distance = (short) atoi(tempBuffer);
		writeString(ptr, tempBuffer);
		ridesStructArray[i].score_u = (short) atoi(tempBuffer);
		writeString(ptr, tempBuffer);
		ridesStructArray[i].score_d = (short) atoi(tempBuffer);
		writeString(ptr, tempBuffer);
		ridesStructArray[i].tip = (short) atof(tempBuffer);
		ridesStructArray[i].comment = loadString(ptr); // e se for null?????????????????

		temp = &(ridesStructArray[i]);
		// check if city is not already in hash table
		if ((array = g_hash_table_lookup(cityTable, city)) == NULL) {
			// if not, insert
			array = g_array_sized_new(FALSE, FALSE, sizeof(RidesStruct *), (1 << 17) + (1 << 15)); // usar ints em vez das struct * ???
			g_array_append_val(array, temp);
			g_hash_table_insert(cityTable, city, array);
		} else {
			// if yes, append to all the other data
			g_array_append_val(array, temp);
			// printf("Adding to %s a ride in %s\n", city, temp->city);
		}

	}

	// for (i = 0; i < count; i++) {
	// 	printf("%s %hd %s %s %hd %hd %hd %.1f %s\n", ridesStructArray[i].date,
	// 	ridesStructArray[i].driver,
	// 	ridesStructArray[i].user,
	// 	ridesStructArray[i].city,
	// 	ridesStructArray[i].distance,
	// 	ridesStructArray[i].score_u,
	// 	ridesStructArray[i].score_d,
	// 	ridesStructArray[i].tip,
	// 	ridesStructArray[i].comment);
	// }

	return ridesStructArray;
}

void freeRidesData(DATA data) {
	RidesData *dataStruct = data;
	RidesStruct **ridesData = dataStruct->ridesArray;
	GHashTable *table = dataStruct->cityTable;
	g_hash_table_destroy(table);
	int i, j;
	RidesStruct *segment, block;
	for (i = 0; i < RIDES_ARR_SIZE; i++) {
		segment = ridesData[i];
		for (j = 0; j < SIZE; j++) {
			block = segment[j];
			free(block.date);
			free(block.user);
			free(block.city);
			free(block.comment);
		}
		free(segment);
	}
	free(ridesData);
	free(dataStruct);
}

RidesStruct * getRideByID(DATA data, int ID) {
	ID -= 1; // para o primeiro passar a ser 0
	int i = ID / RIDES_ARR_SIZE;
	RidesStruct **primaryArray = data,
	* secondaryArray = primaryArray[i],
	* result = &(secondaryArray[ID - SIZE*i]);
	return result;
}

void freeArray(void *data) {
	GArray *array = (GArray *)data;
	g_array_free(array, TRUE);
}
