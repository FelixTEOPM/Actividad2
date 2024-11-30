// Actividad2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <iostream>
#include <iomanip>

using namespace std;

// Función para centrar texto en un ancho específico
static std::string centrarTexto(const std::string& texto, int ancho) {
    int espaciosIzq = (ancho - texto.size()) / 2;
    int espaciosDer = ancho - texto.size() - espaciosIzq;
    return std::string(espaciosIzq, ' ') + texto + std::string(espaciosDer, ' ');
}

int main() {
    SQLHENV hEnv;
    SQLHDBC hDbc;
    SQLRETURN ret;

    // Asignar un gestor de entorno
    ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv);
    ret = SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);

    // Asignar un gestor de conexión
    ret = SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc);

    // Conectarse a la base de datos
    ret = SQLConnect(hDbc, (SQLWCHAR*)L"sqlserver", SQL_NTS, (SQLWCHAR*)L"Username", SQL_NTS, (SQLWCHAR*)L"Password", SQL_NTS);

    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {

        cout << "Conectado a la base de datos exitosamente." << endl;
        
        SQLHSTMT hStmt;
        ret = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

        //Sacar los encabezados del ciclo while principal
        wcout << L"+---------------+----------------------------------------+-----------------+" << endl;
        wcout << L"| No. Empleado  | Nombre Completo                        | Directivo       |" << endl;
        wcout << L"+---------------+----------------------------------------+-----------------+" << endl;

        //Select a la base de datos
        ret = SQLExecDirect(hStmt, (SQLWCHAR*)L"SELECT * FROM Empleados", SQL_NTS);

        if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
            int num_empleado;
            SQLWCHAR name[50];
            SQLWCHAR last_name[50];
            SQLWCHAR slast_name[50];
            SQLWCHAR directivo[50];
            while (SQLFetch(hStmt) == SQL_SUCCESS) {
                SQLGetData(hStmt, 1, SQL_C_LONG, &num_empleado, 0, NULL);
                SQLGetData(hStmt, 2, SQL_C_WCHAR, name, sizeof(name), NULL); 
                SQLGetData(hStmt, 3, SQL_C_WCHAR, last_name, sizeof(last_name), NULL);
                SQLGetData(hStmt, 4, SQL_C_WCHAR, slast_name, sizeof(slast_name), NULL);
                SQLGetData(hStmt, 10, SQL_C_WCHAR, directivo, sizeof(directivo), NULL);
                
                // Concatenar apellido paterno + apellido materno
                wstring last_names = wstring(last_name) + L" " + wstring(slast_name);
                wstring full_name = wstring(name) + L" " + wstring(last_names);
                wstring dir = wstring(directivo);
                wstring respuesta;

                if (dir == L"1") {
                    respuesta = L"Si";
                }
                else
                {
                    respuesta = L"No";
                }
                
                // Imprimir datos de la fila con alineación
                wcout << L"| " << setw(14) << left << num_empleado
                      << L"| " << setw(39) << left << full_name 
                      << L"| " << setw(15) << left << respuesta << L" |" << endl;

            }
            // Imprimir línea final de la tabla
            wcout << L"+---------------+----------------------------------------+-----------------+" << endl;
        }
        // Liberar el manejador de conexión
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);

    }
    else {
        cout << "Fallo la conexion a la base de datos" << endl;
    }

    // Desconectar y liberar gestores de entorno
    SQLDisconnect(hDbc);
    SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
    SQLFreeHandle(SQL_HANDLE_ENV, hEnv);

    return 0;


}