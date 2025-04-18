
# 🐔 Chicken run

**Chicken run** est un mini-jeu multijoueur en LAN développé en C++ avec des sockets POSIX. Jusqu'à 4 joueurs peuvent s'affronter pour aller le plus loin possible sans exploser… ou décider de s'arrêter à temps pour sécuriser leurs points. Parfait pour des soirées chill — avec ou sans boisson 😏

---

## 📁 Structure du projet

```
Chicken run/
├── client/              # Code source du client
│   └── CMakeLists.txt
├── server/              # Code source du serveur
│   └── CMakeLists.txt
├── CMakeLists.txt       # CMake principal (build configurable)
└── (build/)             # Dossier de build généré
```

---

## ⚙️ Compilation

### 🔧 Build des deux exécutables

```bash
mkdir build && cd build
cmake .. 
make
```

### 🛠 Options personnalisées

| Option                | Description                       |
|-----------------------|-----------------------------------|
| `-DBUILD_CLIENT=ON`   | Compiler le client (par défaut)   |
| `-DBUILD_SERVER=ON`   | Compiler le serveur (par défaut)  |

```bash
# Exemple : compiler uniquement le client
cmake -DBUILD_CLIENT=ON -DBUILD_SERVER=OFF ..
make
```

### 📦 Résultat
Après compilation, les exécutables sont automatiquement copiés à la racine du projet :
- `./client_exec`
- `./server_exec`

---

## 🧪 Lancer une partie en LAN

### 👑 Sur l’hôte (serveur)
```bash
./server_exec
```

### 🎮 Sur chaque client (jusqu’à 4 joueurs sur le même Wi-Fi)
```bash
./client_exec
# Entrer l’IP locale du serveur quand demandé (ex: 192.168.1.42)
```

---

## 🥂 Mode jeu à boire (optionnel)
- Tu avances : bois une gorgée
- Tu exploses : bois 2 gorgées + défi
- Tu t’arrêtes : choisis quelqu’un pour boire

---

## 📬 TODO / améliorations possibles

- [ ] Ajout d’objets bonus (ex: gilet pare-explosion)
- [ ] Gestion graphique (SDL / SFML)
- [ ] Liste de défis pour le mode à boire
- [ ] Système de replay ou classement

---

## 💡 Auteurs
- 🧠 Idée : Pablo Levy
