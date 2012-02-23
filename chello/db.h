#pragma once

void db_establish_connection(const char* db);

void db_init_cursor(void* cursor, const char* documents);

void db_close_connection();
