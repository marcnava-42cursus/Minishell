
# 📁 Template de Proyecto 42 - Repositorio Base

> 🇪🇸 Plantilla para proyectos en C en 42 Madrid

---

## 📚 Índice

1. 📦 Estructura del proyecto
2. 🛠️ Uso del Makefile
3. ⚙️ Variables del Makefile
4. 🧪 Comandos útiles
5. 📁 Añadiendo tus archivos
6. 💡 Consejos
7. 🧾 Licencia

---

## 📦 Estructura del proyecto

```bash
.
├── includes/   # Archivos de cabecera (.h)
├── libs/       # Librerías externas (libft, etc.)
├── src/        # Código fuente del proyecto (.c)
├── Makefile    # Sistema de compilación
└── build/      # Directorio generado con archivos .o
```

---

## 🛠️ Uso del Makefile

```bash
make        # Compila el proyecto
make clean  # Elimina archivos .o
make fclean # Elimina .o y el ejecutable
make re     # Limpia y recompila todo
```

---

## ⚙️ Variables del Makefile

| Variable    | Descripción                                 |
|-------------|----------------------------------------------|
| `NAME`      | Nombre del ejecutable                        |
| `CC`        | Compilador (gcc, clang, cc...)               |
| `CFLAGS`    | Flags estándar de compilación                |
| `DFLAGS`    | Flags para debug (opcional)                  |
| `SRCPATH`   | Ruta a los archivos fuente `.c`              |
| `BUILDPATH` | Carpeta para los `.o` generados              |
| `INCLUDES`  | Flags de los headers (`-Iincludes`)          |
| `SRCS`      | Lista de archivos `.c`                       |
| `OBJS`      | Archivos objeto generados automáticamente    |

---

## 📁 Añadiendo tus archivos

1. Coloca tus `.c` en `src/`
2. Coloca tus `.h` en `includes/`
3. Añade los `.c` a `SRCS` en el Makefile:

```make
SRCS := src/main.c src/utils.c
```

---

## 💡 Consejos

- Usa `@` para ocultar comandos en el Makefile.
- Usa `libs/` para librerías como `libft`.
- Puedes definir reglas personalizadas como `bonus`, `test`, `run`, etc.

---

## 🧾 Licencia

MIT License

Copyright (c) 2025

Se concede permiso, de forma gratuita, a cualquier persona que obtenga una copia de este software y los archivos de documentación asociados (el "Software"), para utilizar el Software sin restricción, incluyendo sin limitación los derechos a usar, copiar, modificar, fusionar, publicar, distribuir, sublicenciar y/o vender copias del Software, y permitir a las personas a quienes se les proporcione el Software a hacer lo mismo, sujeto a las siguientes condiciones:

Este aviso de copyright y este permiso se incluirán en todas las copias o partes sustanciales del Software.

EL SOFTWARE SE PROPORCIONA "TAL CUAL", SIN GARANTÍA DE NINGÚN TIPO, EXPRESA O IMPLÍCITA.

---

# 📁 42 Project Template - Base Repository

> 🇺🇸 Template for C projects at 42 School

---

## 📚 Table of Contents

1. 📦 Project Structure
2. 🛠️ Using the Makefile
3. ⚙️ Makefile Variables
4. 🧪 Useful Commands
5. 📁 Adding Your Files
6. 💡 Tips
7. 🧾 License

---

## 📦 Project Structure

```bash
.
├── includes/   # Header files (.h)
├── libs/       # External libraries (libft, etc.)
├── src/        # Project source code (.c)
├── Makefile    # Build system
└── build/      # Directory for generated object files (.o)
```

---

## 🛠️ Using the Makefile

```bash
make        # Compile the project
make clean  # Remove object files
make fclean # Remove object files and binary
make re     # Clean and rebuild everything
```

---

## ⚙️ Makefile Variables

| Variable    | Description                                 |
|-------------|---------------------------------------------|
| `NAME`      | Name of the final executable                |
| `CC`        | Compiler (gcc, clang, cc...)                |
| `CFLAGS`    | Standard compiler flags                     |
| `DFLAGS`    | Debug flags (optional)                      |
| `SRCPATH`   | Path to `.c` source files                   |
| `BUILDPATH` | Folder for generated `.o` files             |
| `INCLUDES`  | Header path flags (`-Iincludes`)            |
| `SRCS`      | List of `.c` files                          |
| `OBJS`      | Auto-generated object file list             |

---

## 📁 Adding Your Files

1. Put your `.c` files in `src/`
2. Put your `.h` files in `includes/`
3. Add your `.c` files to the `SRCS` variable in the Makefile:

```make
SRCS := src/main.c src/utils.c
```

---

## 💡 Tips

- Use `@` in Makefile rules to hide commands.
- Use `libs/` for dependencies like `libft`.
- You can define custom rules like `bonus`, `test`, `run`, etc.

---

## 🧾 License

MIT License

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell    
copies of the Software, and to permit persons to whom the Software is         
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included       
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR    
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,      
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE   
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER        
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING       
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS  
IN THE SOFTWARE.
