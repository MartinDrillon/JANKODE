# JANKODE V3

Firmware pour Teensy 4.1 - Scanner MIDI 120 touches pour Clavicore 1.0

## Architecture

Le projet propose deux modes de build pour l'acquisition des données :

### Mode DEFAULT (V3_DMA) - `USE_V3_DMA=1`
- **PIT Timer** : Cadence isochrone des steps (10 kHz = ~625 scans/s)
- **ADC_ETC** : Chaîne de 8 conversions simultanées par step
- **eDMA** : Transfert automatique vers buffers ping/pong
- **Performance** : CPU déchargée, acquisition totalement matérielle

### Mode ALT (V2_SYNC) - `USE_V3_DMA=0`  
- **Fallback logiciel** via lib ADC (analogSynchronizedRead)
- **Compatibilité** : Mode de secours pour débogage
- **Flexibilité** : Paramétrage ADC plus simple

## Structure Hardware

### MUX Configuration
- **8 MUX CD4067** (16 canaux chacun)
- **120 touches** réparties sur MUX1-8
- **Trous** : MUX1 {I4,I6,I9,I11}, MUX8 {I1,I3,I12,I14}

### Pins Teensy 4.1
```
Sélecteurs GROUP A (MUX1-4): 27,26,28,29 (S0,S1,S2,S3)
Sélecteurs GROUP B (MUX5-8): 38,39,37,36 (S0,S1,S2,S3)
Analogiques MUX1-8        : 33,32,31,30,45,44,43,42
MIDI Routing              : 4,5 (mode selection)
```

### Capteurs
- **DRV5055** : Capteurs Hall ratiométriques (VCC/2 centrés)
- **OPA365** : Buffer/anti-alias 20 kHz BW
- **Vélocité** : Algorithme dv/dt avec FSM IDLE→TRACKING→HELD

## MIDI Routing (Pins 4 & 5)

| Pin4 | Pin5 | Mode |
|------|------|------|
| LOW  | LOW  | Device Only (usbMIDI) |
| HIGH | LOW  | Host Only (USBHost_t36) |
| LOW  | HIGH | Device Only (fallback) |
| HIGH | HIGH | Host + Device |

## Build & Flash

```bash
# Mode DMA (défaut)
pio run -e teensy41
pio run -e teensy41 -t upload

# Mode SYNC (fallback)  
pio run -e teensy41_sync
pio run -e teensy41_sync -t upload

# Monitor série
pio device monitor
```

## Structure Projet

```
├── platformio.ini          # Config PlatformIO (2 environnements)
├── include/
│   ├── JANKODE.h           # Header principal
│   ├── keys.h              # Config pins & buffers
│   ├── keymap.h            # LUT 120 touches & FSM vélocité
│   ├── calibration.h       # Seuils ADC 2.3V/3.2V
│   ├── midi_routing.h      # Abstraction MIDI Device/Host
│   └── scan_dma.h          # API mode DMA (V3 uniquement)
├── src/
│   ├── main.cpp            # Boucle principale (dispatch V3/V2)
│   ├── keys.cpp            # Init pins & tables
│   ├── keymap.cpp          # LUT + FSM vélocité
│   ├── calibration.cpp     # Seuils par défaut
│   ├── midi_routing.cpp    # Routage MIDI selon pins 4/5
│   ├── scan_dma.cpp        # Implémentation DMA (registre-level)
│   └── scan_sync.cpp       # Fallback ADC lib (V2)
```

## Calibration

Seuils basés sur DRV5055 + OPA365 :
- **ThresholdLow** : 716 LSB (2.3V) - Détection pression
- **ThresholdHigh** : 993 LSB (3.2V) - Pression confirmée  
- **ThresholdRelease** : 943 LSB (High-50) - Hystérésis relâchement

## Performance

- **V3 DMA** : ~625 scans/s, <5% CPU, jitter <1µs
- **V2 SYNC** : ~400 scans/s, ~15% CPU, jitter variable
- **Latence** : <2ms détection, vélocité dv/dt précise

## Notes Techniques

- **Code Gray** : Séquence anti-glitch pour sélecteurs MUX
- **Sink Write** : Table W[] évite les branches (trous → dummySink)
- **Double Buffer** : Ping/pong pour acquisition continue
- **T_SETTLE** : 1µs stabilisation après commutation MUX

## Compatibilité

- **Teensy 4.1** (i.MX RT1062)
- **PlatformIO** + Arduino Framework
- **USB MIDI** (Device) + **USBHost_t36** (Host)
- **ADC lib** (Pedro Villanueva) pour fallback V2
