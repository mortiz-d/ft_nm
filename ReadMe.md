# Análisis de herramientas para la inspección de archivos ELF

Este README proporciona información sobre varias herramientas utilizadas para inspeccionar y analizar archivos binarios, como los archivos ELF (Executable and Linkable Format), y sus componentes, incluyendo las tablas de símbolos y los encabezados de sección. Las herramientas que se cubren son `nm`, `readelf`, `objdump`, y `hexdump`, entre otras, para permitir una inspección profunda de estos archivos.

---

## 1. Introducción

Los archivos ELF son utilizados principalmente en sistemas Unix/Linux y contienen la información necesaria para que un sistema operativo ejecute programas y bibliotecas. El formato ELF está compuesto por varias secciones y tablas, cada una con diferentes propósitos, como almacenamiento de código, datos y símbolos.

Entre las herramientas más útiles para examinar estos archivos y obtener detalles sobre sus secciones, símbolos, dependencias y más, se incluyen:

- **`nm`**: Muestra las tablas de símbolos de un archivo binario.
- **`readelf`**: Muestra información detallada sobre los encabezados de ELF, secciones y símbolos.
- **`objdump`**: Proporciona una forma de desensamblar el código y ver la estructura interna de los binarios.
- **`hexdump`**: Permite visualizar el contenido binario de los archivos en diferentes formatos.

---

## 2. Herramientas de Inspección

### `nm` - Visualización de Símbolos en Archivos Binarios

La herramienta `nm` se utiliza para listar los símbolos definidos en un archivo binario, y su salida permite identificar las funciones y variables globales, así como las definiciones no resueltas (símbolos indefinidos) ordenadas alfabeticamente.

#### Comandos de este proyecto:
- `nm -p <archivo>`: Muestra los símbolos en el orden en que aparecen en el archivo.
- `nm -g <archivo>`: Muestra solo los símbolos externos (globales).
- `nm -a <archivo>`: Muestra todos los símbolos, incluidos los utilizados solo para depuración.
- `nm -u <archivo>`: Muestra solo los símbolos indefinidos (no resueltos).
- `nm -r <archivo>`: Invierte el orden de la salida (puede ser útil al analizar dependencias).


Referencias adicionales sobre cómo interpretar los símbolos y su información:  
- [Oracle ELF Format Documentation](https://docs.oracle.com/cd/E19683-01/816-1386/chapter6-94076/index.html)

### `readelf` - Análisis del Formato ELF

La herramienta `readelf` permite inspeccionar la estructura interna de los archivos ELF, proporcionando información sobre los encabezados, las secciones, las tablas de símbolos y más.

#### Comandos comunes de `readelf`:
- `readelf -h <archivo>`: Muestra el encabezado del archivo ELF.
- `readelf -l <archivo>`: Muestra los encabezados de los segmentos.
- `readelf -S <archivo>`: Muestra los encabezados de las secciones.
- `readelf -s <archivo>`: Muestra la tabla de símbolos.
- `readelf -r <archivo>`: Muestra la información de reubicación de las secciones.


**Estructura del encabezado ELF**:
- [Elf Header Documentation](https://docs.oracle.com/cd/E19120-01/open.solaris/819-0690/chapter6-43405/index.html)

```c
typedef struct {
    unsigned char   e_ident[EI_NIDENT]; // Los primeros bytes, contienen las "palabras mágicas"
    Elf64_Half      e_type;             // Tipo de archivo (ej. ejecutable, compartido)
    Elf64_Half      e_machine;          // Arquitectura (x86_64, ARM, etc.)
    Elf64_Word      e_version;          // Versión del archivo ELF
    Elf64_Addr      e_entry;            // Dirección de entrada (punto de inicio de ejecución)
    Elf64_Off       e_phoff;            // Desplazamiento de la tabla de encabezados del programa
    Elf64_Off       e_shoff;            // Desplazamiento de la tabla de encabezados de secciones
    Elf64_Word      e_flags;            // Flags del archivo ELF
    Elf64_Half      e_ehsize;           // Tamaño del encabezado ELF
    Elf64_Half      e_phentsize;        // Tamaño de cada entrada en la tabla de encabezados del programa
    Elf64_Half      e_phnum;            // Número de entradas en la tabla de encabezados del programa
    Elf64_Half      e_shentsize;        // Tamaño de cada entrada en la tabla de encabezados de secciones
    Elf64_Half      e_shnum;            // Número de entradas en la tabla de encabezados de secciones
    Elf64_Half      e_shstrndx;         // Índice de la tabla de cadenas de nombres de secciones
} Elf64_Ehdr;
```
**Estructura del encabezado ELF**:
- [Elf Sections Documentation](https://docs.oracle.com/cd/E19683-01/816-1386/chapter6-94076/index.html)
```c
typedef struct {
    Elf64_Word      sh_name;                // Nombre de la sección (índice en la tabla de cadenas)
    Elf64_Word      sh_type;                // Tipo de sección
    Elf64_Xword     sh_flags;               // Flags de la sección
    Elf64_Addr      sh_addr;                // Dirección virtual donde comienza la sección
    Elf64_Off       sh_offset;              // Desplazamiento desde el inicio del archivo
    Elf64_Xword     sh_size;                // Tamaño de la sección en bytes
    Elf64_Word      sh_link;                // Enlace a otra sección
    Elf64_Word      sh_info;                // Información adicional
    Elf64_Xword     sh_addralign;           // Alineación de la sección
    Elf64_Xword     sh_entsize;             // Tamaño de las entradas (si la sección es una tabla)
} Elf64_Shdr;
```

**Estructura de los simbolos**:
- [Elf Symbols Documentation](https://docs.oracle.com/cd/E19683-01/816-1386/chapter6-79797/index.html)
```c
typedef struct {
    Elf64_Word      st_name;                // Nombre del símbolo (índice en la tabla de cadenas)
    unsigned char   st_info;                // Tipo y enlace del símbolo (STB_LOCAL, STB_GLOBAL, STT_FUNC, etc.)
    unsigned char   st_other;               // Visibilidad del símbolo
    Elf64_Half      st_shndx;               // Índice de la sección que contiene el símbolo
    Elf64_Addr      st_value;               // Dirección donde se encuentra el símbolo
    Elf64_Xword     st_size;                // Tamaño del símbolo
} Elf64_Sym;
```

Los símbolos pueden tener diferentes tipos y atributos, tales como:
- **STB_LOCAL**: Símbolos locales, solo visibles dentro del archivo.
- **STB_GLOBAL**: Símbolos globales, visibles para otros archivos.
- **STB_WEAK**: Símbolos débiles, con menor prioridad que los símbolos globales.

**Tipos de símbolos**:
- **STT_FUNC**: Funciones.
- **STT_OBJECT**: Objetos de datos (como variables).
- **STT_SECTION**: Símbolos asociados a secciones.