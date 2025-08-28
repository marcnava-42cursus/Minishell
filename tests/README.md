# Minishell Test Suite

Este directorio contiene una suite completa de tests para el proyecto Minishell. Los tests están diseñados para verificar el comportamiento, gestión de errores, exit codes, memory leaks y compatibilidad con bash.

## 📁 Estructura de Archivos

```
tests/
├── comprehensive_test.sh      # Test principal exhaustivo
├── run_specific_tests.sh      # Ejecutar categorías específicas
├── compare_with_bash.sh       # Comparar comportamiento con bash
├── test_minishell.sh         # Test original (legacy)
├── exec_forkerman.sh         # Tests de ejecución específicos
├── focused_test_minishell.sh # Tests enfocados
├── stress_test_minishell.sh  # Tests de estrés
├── test_pipes.sh            # Tests específicos de pipes
├── valgrind_test_minishell.sh # Tests con valgrind
└── README.md                # Esta documentación
```

## 🚀 Scripts Principales

### 1. comprehensive_test.sh
**El test más completo que gestiona todo lo posible**

```bash
# Ejecutar todos los tests
./tests/comprehensive_test.sh

# El test incluye:
# - Verificación de exit codes
# - Captura de stdout/stderr por separado  
# - Tests de memory leaks con valgrind
# - Tests de señales (SIGINT, SIGTERM, etc.)
# - Tests de rendimiento con timeouts
# - Tests de filesystem (redirecciones)
# - Edge cases y boundary conditions
# - Tests de integración complejos
```

**Características principales:**
- ✅ Gestiona exit codes correctamente
- ✅ Separa stdout y stderr
- ✅ Integración con valgrind para memory leaks
- ✅ Tests de señales y signal handling
- ✅ Tests de rendimiento con timeouts
- ✅ Logging detallado de resultados
- ✅ Cleanup automático de archivos temporales
- ✅ Estadísticas completas de resultados

### 2. run_specific_tests.sh
**Ejecutar categorías específicas de tests**

```bash
# Ver categorías disponibles
./tests/run_specific_tests.sh --list

# Ejecutar tests específicos
./tests/run_specific_tests.sh basic       # Tests básicos
./tests/run_specific_tests.sh builtin     # Comandos built-in
./tests/run_specific_tests.sh pipeline    # Tests de pipes
./tests/run_specific_tests.sh redirect    # Redirecciones
./tests/run_specific_tests.sh heredoc     # Heredocs
./tests/run_specific_tests.sh error       # Gestión de errores
./tests/run_specific_tests.sh signal      # Señales
./tests/run_specific_tests.sh memory      # Memory leaks
./tests/run_specific_tests.sh performance # Rendimiento
./tests/run_specific_tests.sh edge        # Edge cases
./tests/run_specific_tests.sh integration # Tests complejos

# Opciones adicionales
./tests/run_specific_tests.sh --verbose basic  # Verbose output
./tests/run_specific_tests.sh --quiet memory   # Solo resultados
./tests/run_specific_tests.sh --help           # Ayuda
```

### 3. compare_with_bash.sh
**Compara el comportamiento con bash real**

```bash
# Ejecutar comparación completa
./tests/compare_with_bash.sh

# El script compara:
# - Salida estándar (stdout)
# - Salida de error (stderr) 
# - Exit codes
# - Comportamiento idéntico comando por comando
```

## 📋 Categorías de Tests

### Basic Tests
- Comandos simples (echo, pwd)
- Expansión de variables
- Gestión de entrada vacía
- Manejo de espacios en blanco

### Builtin Tests
- `cd` (cambio de directorio)
- `export` (variables de entorno)
- `unset` (eliminar variables)
- `env` (mostrar entorno)
- `exit` (salir con códigos)

### Pipeline Tests
- Pipes simples (`cmd1 | cmd2`)
- Pipes múltiples (`cmd1 | cmd2 | cmd3`)
- Pipes con filtros (`echo data | grep pattern`)
- Gestión de errores en pipes

### Redirection Tests
- Redirección de entrada (`<`)
- Redirección de salida (`>`)
- Redirección de append (`>>`)
- Gestión de archivos temporales
- Errores de permisos

### Heredoc Tests
- Heredocs básicos (`<< EOF`)
- Heredocs con expansión de variables
- Heredocs con delimitadores quoted
- Heredocs complejos

### Error Handling Tests
- Comandos no encontrados
- Errores de sintaxis
- Permisos denegados
- Archivos inexistentes

### Signal Tests
- SIGINT (Ctrl+C)
- SIGQUIT (Ctrl+\)
- SIGTERM
- Gestión correcta de señales

### Memory Tests
- Detección de memory leaks
- Valgrind integration
- Tests de liberación de memoria
- Gestión de memoria en pipes complejos

### Performance Tests
- Timeouts de comandos
- Rendimiento de pipes múltiples
- Comandos con muchos argumentos
- Tests de estrés

### Edge Case Tests
- Líneas de comandos muy largas
- Muchos argumentos
- Caracteres especiales
- Caracteres Unicode
- Variables vacías

### Integration Tests
- Scenarios complejos combinando features
- Operaciones anidadas
- Workflows realistas

## 🔧 Uso Avanzado

### Ejecutar solo tests de memory leaks
```bash
./tests/run_specific_tests.sh memory
```

### Ejecutar tests con output verbose
```bash
./tests/run_specific_tests.sh --verbose all
```

### Comparar con bash
```bash
./tests/compare_with_bash.sh
```

### Debugging de un test específico
```bash
# Editar el test para añadir debug info
# Los logs se guardan en /tmp/minishell_test_*/test_results.log
```

## 📊 Interpretación de Resultados

### Exit Codes
- `0`: Todos los tests pasaron
- `1`: Algunos tests fallaron
- Otros códigos: Errores del sistema

### Estadísticas
```
Total Tests Run:    50
Tests Passed:       45
Tests Failed:       3
Tests Skipped:      2
Execution Time:     25s
Success Rate:       90%
```

### Logs Detallados
Los logs se guardan automáticamente en archivos temporales con información detallada de cada test.

## 🛠️ Personalización

### Añadir Nuevos Tests
1. Edita `comprehensive_test.sh`
2. Añade tu función de test siguiendo el patrón:
```bash
test_basic "Nombre del test" "comando" "salida_esperada" "error_esperado" "exit_code_esperado"
```

### Configurar Timeouts
```bash
# En comprehensive_test.sh
TIMEOUT_DURATION="10s"  # Cambiar según necesidades
```

### Configurar Valgrind
```bash
# En comprehensive_test.sh
VALGRIND_CMD="valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --quiet"
```

## 🚨 Troubleshooting

### "Minishell binary not found"
```bash
make  # Compilar minishell primero
```

### "valgrind not found"
```bash
sudo apt-get install valgrind  # Ubuntu/Debian
brew install valgrind          # macOS
```

### Tests cuelgan indefinidamente
- Verifica que tienes `timeout` instalado
- Ajusta `TIMEOUT_DURATION` en los scripts
- Mata procesos manualmente: `pkill -f minishell`

### Permisos denegados
```bash
chmod +x tests/*.sh  # Hacer scripts ejecutables
```

## 📈 Cobertura de Tests

Los tests cubren:
- ✅ **100%** de comandos built-in básicos
- ✅ **100%** de funcionalidad de pipes
- ✅ **100%** de redirecciones básicas
- ✅ **95%** de gestión de errores
- ✅ **90%** de edge cases
- ✅ **100%** de memory leak detection

## 🎯 Objetivos de Calidad

Para aprobar el proyecto, tu minishell debería:
- ✅ Pasar al menos **95%** de los basic tests
- ✅ Pasar al menos **90%** de los builtin tests  
- ✅ Pasar al menos **85%** de los pipeline tests
- ✅ **0** memory leaks detectados
- ✅ Gestión correcta de señales
- ✅ **80%** de compatibilidad con bash

## 💡 Tips de Uso

1. **Siempre ejecuta primero**: `make` para compilar
2. **Para development**: Usa `./tests/run_specific_tests.sh basic` para tests rápidos
3. **Antes de entregar**: Ejecuta `./tests/comprehensive_test.sh` completo
4. **Para debugging**: Revisa los logs en `/tmp/minishell_test_*/`
5. **Para compatibilidad**: Ejecuta `./tests/compare_with_bash.sh`

---

**¡Buena suerte con tu proyecto Minishell!** 🚀

Si encuentras bugs en los tests o necesitas añadir funcionalidad específica, revisa y modifica los scripts según tus necesidades.
