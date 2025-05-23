
```
        ----       _____                 _      ____                  _   
     `/\(  )      / ____|               | |    / __ \                | |  
    | ^  \       | |  __ _ __ __ _ _ __ | |__ | |  | |_   _  ___  ___| |_ 
    | -  | \o    | | |_ | '__/ _` | '_ \| '_ \| |  | | | | |/ _ \/ __| __|
     /\\ /       | |__| | | | (_| | |_) | | | | |__| | |_| |  __/\__ \ |_ 
      |  |  ___   \_____|_|  \__,_| .__/|_| |_|\___\_\\__,_|\___||___/\__|
      |  | |___|                    |_|                                   
```
<h1 style="text-align: center;"> <b> GraphQuest </b> </h1>

**GraphQuest** es una aventura. En esta aventura, controlarás a un **bandido perdido**.

<details>
<summary> <b> Mapa del Juego (Spoilers)</b> </summary>

<div align="center">

| Entrada Principal | Librería | Caverna Oscura  | Estudio |
|:-----------------:|:--------:|:---------------:|:-------:|
| Cocina            | Comedor  | Sótano          | Taller  |
| Jardín            | Pasillo  | Sala del Tesoro | Almacén |
| Dormitorio        | Corredor | Cámara Secreta  | Salida  |

</div>
</details>

<br/>

### Requerimientos
- Tener un compilador de C (como **gcc**) instalado.
    - Para **Windows**, es recomendable utilizar **MinGW**.
    - Para sistemas basados en **Arch Linux**, se puede ejecutar en la terminal el comando `sudo pacman -S gcc` para instalarlo de los **paquetes oficiales** de Arch.

### Compilación y ejecución
<details>
<summary>
Para compilarlo, dentro del sistema operativo <b>Windows</b> usando <b>Visual Studio Code</b>: </summary>

1. Descargar este **repositorio** como archivo `zip` (haciendo **click** en la sección `code`, y apretando el botón de `Descargar ZIP`).
2. Abrir el **explorador de archivos** y navegar hasta el archivo `zip` para descomprimirlo. Una vez descomprimido, abrir el directorio con los archivos del programa.
3. Abrir el archivo `main.c` del `zip` en **Visual Studio Code**.
4. Dentro de **Visual Studio Code**, abrir el **terminal** y dirigirse a la dirección del repositorio, para poder compilar el programa.
5. Ejecutar el siguiente comando: `gcc headers/*.c *.c -o programa.exe`.
6. Abrir el archivo `programa.exe`, o escribir la línea `./programa.exe` en **Visual Studio Code** para ejecutarlo.
</details>

---
<details>
<summary>
Una forma alternativa de compilación, funcional para <b>Windows</b> y <b>Linux</b> (<b>Arch Linux</b> <i>testeado</i>): </summary>

1. Descargar el **repositorio** como archivo `zip`.
2. Abrir el **explorador de archivos** de su preferencia y navegar hasta encontrar el archivo `zip` para descomprimirlo.
3. Una vez descomprimido, buscar el directorio donde se ubica la carpeta descomprimida y buscar abrir el **terminal** en algunos de los sistemas operativos.

    - En **Windows**, se puede hacer **click derecho** en el directorio para abrir el menú de opciones y seleccionar para **abrir en Terminal**. Alternativamente, abrir **PowerShell** o **Línea de Comandos**, copiar la dirección del repositorio (la ruta `C:\Users\...`) y ejecutar el comando `cd "C:\Users\..."`.
    - En **Linux**, se puede ejecutar, dentro de la **terminal** de su preferencia, el comando `cd`, similar a como se hace en **Windows**. Es necesario copiar la dirección del repositorio (la ruta `"/home/$USER/..."`) y ejecutar el comando `cd "/home/$USER/..."` para acceder a la ruta del programa.
4. Ejecutar el comando: `gcc headers/*.c *.c -o programa`.
5. Escribir en la misma **terminal** el comando `./programa` para ejecutar la aplicación.
</details>

### Funciones del programa

### Problemas generales

### Ejemplos de ejecución