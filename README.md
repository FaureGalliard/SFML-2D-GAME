# 🎮 Save the Valley

Un juego sandbox en 2D desarrollado en C++ con SFML, donde exploras un mundo proceduralmente generado y  combates enemigos.

## 📋 Características

### 🌍 Mundo Procedural
- Generación infinita de chunks con sistema de carga/descarga dinámica
- Múltiples biomas con diferentes tipos de terreno
- Sistema de autotiling para transiciones suaves entre tiles
- Objetos del mundo con colisiones (árboles, rocas, estructuras)

### ⚔️ Sistema de Combate
- Múltiples tipos de ataque (espada, hacha, martillo)
- Sistema de daño y salud para héroe y enemigos
- Animaciones de ataque, daño y muerte
- Invulnerabilidad temporal tras recibir daño
- Hitboxes dinámicas según el tipo de ataque

### 🤖 Inteligencia Artificial
- Sistema de estados para enemigos (Idle, Wandering, Hunting)
- Detección automática del jugador dentro de un rango
- Persecución inteligente con evasión de obstáculos
- Patrullaje aleatorio cuando no hay objetivo
- Ataques automáticos al alcanzar al jugador

### 🎨 Sistema de Renderizado
- Renderizado optimizado por chunks visibles
- Sistema de capas (terreno, objetos, entidades)
- Animaciones por sprite sheets con múltiples capas
- Cámara suave que sigue al jugador
- Modo debug para visualizar colisiones

### 🎮 Controles y Movimiento
- Movimiento en 8 direcciones con WASD
- Sprint con Shift
- Sistema de colisiones con objetos del mundo
- Múltiples acciones (atacar, minar, cavar, regar)

## 🛠️ Tecnologías

- **Lenguaje**: C++17
- **Gráficos**: SFML 2.6+
- **Build System**: CMake 3.20+
- **Assets**: Sunnyside World Asset Pack
