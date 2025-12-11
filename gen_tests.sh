#!/usr/bin/env bash
set -euo pipefail

# === Parámetros configurables ===
GEN="./gen"          # ruta al ejecutable del generador
OUT_DIR="standar_cases"      # carpeta donde guardar los .txt
BASE_NAME="test"     # prefijo del archivo: caso_01.txt, caso_02.txt, ...
NUM_FILES=10         # cuántos archivos generar

Q=100000            # número de operaciones
MX=1000000000           # máximo valor de coordenadas y altura h

# === Crear carpeta de salida si no existe ===
mkdir -p "$OUT_DIR"

# === Loop de generación ===
for ((i = 1; i <= NUM_FILES; i++)); do
    # índice con padding de 2 dígitos (01, 02, ..., 10, etc.)
    printf -v idx "%02d" "$i"
    FILE="$OUT_DIR/${BASE_NAME}_${idx}.txt"

    echo "Generando $FILE  (q=$Q, mx=$MX)..."
    # Le pasamos "q mx" al generador por stdin y redirigimos la salida al archivo
    echo "$Q $MX" | "$GEN" > "$FILE"
done

echo "Listo. Archivos generados en '$OUT_DIR/'."

