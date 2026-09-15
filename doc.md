# Horizon-Next — Technical Documentation

**Version: 2026.1.0 Beta**

Horizon-Next (namespace `hn::`) is a custom C++20 game engine, built on top of Ogre3D for rendering, a 3D physics backend, and Lua for scripting. It exposes a stable API through a DLL (`ENGINE_API`), an Actor/Component system, a lightweight reflection layer, a dynamically-loaded plugin system, and a module system that lets a game register its gameplay classes without recompiling the engine.

This document presents the engine's internal architecture as it exists in the source code, module by module.

---

## 1. Architecture overview

```
hn::Engine                 → entry point, orchestrates everything (rendering, plugins, Lua, logs)
 ├── rendering_interface    → Ogre3D backend (private, internal to engine.cpp)
 ├── hn::LuaVM               → embedded Lua VM
 ├── hn::Logger               → extensible logging system
 ├── hn::Private::SysPlugin   → dynamic plugin loading (.dll)
 └── hge::HGE_Level            → current level, holds the Actors

hge::HGE_Object              → base class for every reflectable entity (properties)
 └── hge::HGE_Actor            → world object (transform, physics, components)
      ├── hge::HGE_Character    → actor with character movement/physics
      └── hge::HGE_Component (composed, not inherited)
           ├── hge::HGE_SceneComponent → component with a relative transform
           │    ├── hge::HGE_Camera
           │    ├── hge::HGE_Light
           │    ├── hge::HGE_Sprite
           │    └── hge::HGE_Shape (base class for collision shapes)
           │         ├── Box, Capsule, Circle
```

Two namespaces coexist:
- **`hn::`** — the engine itself: `Engine`, `Logger`, `LuaVM`, the plugin/module system, filesystem, ini parser.
- **`hge::`** — the gameplay framework exposed to games: `HGE_Object`, `HGE_Actor`, `HGE_Level`, components, input, player controller.

---

## 2. The core: `hn::Engine`

Defined in `src/core/engine.h` / `engine.cpp`. This is the central class, non-copyable, instantiated once in `main()`.

```cpp
hn::Engine engine("default_engine.ini", /*editor=*/true, /*logger=*/nullptr);
engine.StartMainLoop();
```

### Responsibilities
- **Log initialization** (`InitLog`), intercepting `std::cout` through `EngineStreambuf`.
- **Filesystem initialization** (`InitFilesystem`) — raw files or a packaged archive, depending on editor mode.
- **Configuration loading** via `HN_Ini` (an `.ini` file, e.g. `default_engine.ini`).
- **Rendering backend initialization** (`rendering_interface`, private to `engine.cpp`): creates the Ogre context (`OgreBites::ApplicationContext`), the `Ogre::Root`, the `SceneManager`, the overlay system, and a `TrayManager` for debug/viewport UI.
- **Lua VM initialization** (`LuaVM`) and registration of the standard functions.
- **Plugin loading**: scans the `Plugins/<category>/<name>/*.dll` folder, loads each valid DLL through `Private::SysPlugin`, and calls `Init()` on every plugin.
- **Main loop** (`StartMainLoop`): delegates rendering to `Ogre::Root::startRendering()`. Per-frame tick logic lives in `HNFrameListener::frameStarted` (internal to `engine.cpp`): updates the Lua VM, the actors (`Update` always, `Tick` only if `game_tick_enabled_`), the plugins, and clears expired viewport messages.

### Main public API

| Method | Role |
|---|---|
| `StartGame()` / `EndGame()` | Enables/disables gameplay simulation (`game_tick_enabled_`), calls `StartGame`/`EndGame` on every actor of the current level |
| `OpenLevel(HGE_Level*)` / `UnloadCurrentLevel()` | Switches the current level (destroys the previous one) |
| `GetDeltaTime()` | Delta time of the current frame |
| `GetRenderingBackend()` | Opaque pointer (`void*`) to `rendering_interface`, used internally by rendering components |
| `SetCustomLoggerSubsystem(Logger*)` / `Log(...)` | Pluggable logging system |
| `GetLuaVM()` | Access to the Lua VM |
| `hn::GetEngineVersion()` | Returns the version string (`ENGINE_VERSION_STR`, defined in CMake) |

Rendering, plugins, and Lua are all cleanly torn down in `Engine`'s destructor.

---

## 3. Object and reflection system: `hge::HGE_Object`

Defined in `src/core/object.h`. This is the base class for **every** entity in the engine (Actors and Components both derive from it indirectly).

### Lifecycle
```cpp
virtual void Init();
virtual void StartGame();  // start of gameplay simulation
virtual void EndGame();    // end of gameplay simulation
virtual void Tick(double dt);    // gameplay only
virtual void Update(double dt);  // every frame, even outside simulation
```

### Reflection (properties)
Every object owns an `ordered_map<std::string, HGE_Property>` that maps a name to a typed pointer (`std::variant` of `int*, float*, bool*, std::string*, HGE_Vec2*, HGE_Vec3*, HGE_Vec4*, HGE_Transform*, HGE_Path*`). A property can be `Exposed` (external read/write, e.g. editor) or `Editable`.

The `HPROPERTY` macro declares a reflectable property and attaches it to an `Observable`, which fires a callback whenever the value changes (checked every `Tick`):

```cpp
HPROPERTY(life, hge::Exposed);
```

Generic accessors:
```cpp
T GetPropertyValue(const char* name);
void SetPropertyValue(const char* name, T val);
void SetPropertyValueStr(const char* name, const char* val); // string → typed conversion
bool PropertyExists(const char* name);
```

---

## 4. Actors and levels

### `hge::HGE_Actor` (`src/core/actor.h`)
An `HGE_Actor` is a world `HGE_Object`: a transform (`HGE_Transform transform`), a physics mode (`NONE/STATIC/DYNAMIC/KINEMATIC`, backed internally by an opaque physics handle), and a collection of components (`std::unordered_map<std::string, std::unique_ptr<HGE_Component>>`).

Components are added via:
```cpp
HGE_Component* AddComponent(const char* name, ctor);
// or through the convenience macro:
HCOMPONENT("Sprite", HGE_Sprite);
```

Exposed events (`HEventDispatcher`): `ED_transform_modified`, `ED_physics_mode_changed`, `ED_possess_state_changed` (with `OnPossessed(int)` / `OnUnpossessed(int)` for multiplayer handling).

### `hge::HGE_Character` (`src/core/character.h`)
A specialization of `HGE_Actor` with character-style movement (platformer): gravity, max fall velocity, jump force, max horizontal velocity, ground/ceiling detection (`grounded_`, `on_ceiling_`), with `Jump()` and `MoveX(float)`. Internally uses a capsule shape component as its collider.

### `hge::HGE_Level` (`src/core/level.h`)
A container of actors. `LoadFromFile(path)` loads a scene (XML format, via tinyxml2, linked in the CMake build). `SpawnActor(className)` instantiates an actor registered in the gameplay factory. `GetActors()`, `GetActorFromID(id)`, `CountActorsOfClass(...)`.

---

## 5. Component system

Base class: `hge::HGE_Component` (`src/components/base_component.h`) — an `HGE_Object` owned by an `HGE_Actor` (`parent_`). `Init()` is locked there (`final {}`): all logic goes through the constructor instead.

`hge::HGE_SceneComponent` adds a `relative_transform` and absolute-transform accessors (`GetAbsoluteTransform/Location/Rotation/Scale`), with a `TransformModified()` virtual hook.

Concrete components available:
- **`HGE_Camera`** (`camera.h`) — `Perspective`/`Orthographic`, `fov_ortho_height`, rendering backend hidden behind `BackendCamera*`.
- **`HGE_Light`** (`light.h`) — color, attenuation, intensity, each with its own `*Modified()` callback.
- **`HGE_Sprite`** (`sprite.h`) — a `material` (string), hidden rendering backend.
- **`HGE_Shape`** (`shapes/shape.h`) — base class for collision shapes (density, friction, bounciness, and contact-event toggling). Concrete subclasses: `HGE_ShapeBox`, `HGE_ShapeCapsule`, `HGE_ShapeCircle`.

---

## 6. Event system and reactive data

- **`hge::HEventDispatcher<Args...>`** (`data/event_dispatcher.h`) — a minimal pub/sub: `Subscribe(fn) → id`, `Unsubscribe(id)`, `Call(args...)`. Used for all internal Actor/Component events.
- **`Observable<T>` / `IObservable`** (`data/observable.h`) — wraps a reference and runs a callback whenever the value changes between two calls to `Tick()`. This is the mechanism behind `HPROPERTY`.
- **`hn::HN_Ini`** (`data/ini_parser.h`) — a templated `.ini` file parser (`Get<T>`, `GetEntries<T>`, `SetKeyValue<T>`, `SaveIniFile()`), based on `inih` (third-party).
- **`ordered_map`** (`data/ordered_map.h`) — an insertion-order-preserving map, used for properties (likely to keep a stable display order in an editor).

---

## 7. Module system (a game's gameplay code)

A **module** is a game DLL that registers its gameplay classes (custom `HGE_Object`s, typically `HGE_Actor`s) into the engine's global factory, so that `HGE_Level::SpawnActor("MyClass")` can instantiate them by name.

```cpp
// game/src/module.cpp
#include "player.h"
#include "../../src/modules/module.h"

HGE_LINK_MODULE
(
    HGE_MODULE_REGISTER(Player);
)
```

- `HGE_MODULE_REGISTER(Class)` adds `{"Class", []() { return new Class(); }}` to a local factory.
- `HGE_LINK_MODULE(...)` generates the exported symbols `FactoryRegisterClasses` / `FactoryUnregisterClasses` (`__declspec(dllexport)`), called by the engine through `hn::Private::SysModule` (`modules/Private/SystemModule.h`), which loads the DLL and dynamically resolves both symbols.

Minimal example of a gameplay class (`game/src/player.h`):
```cpp
class Player : public hge::HGE_Actor {
public:
    int life = 100;
    hge::InputAction jump_action = "jump";
    hge::InputAxis1D move_forward = "move_x";

    Player() { HPROPERTY(life, hge::Exposed); }

private:
    void ProcessInput(double dt) override {
        if (jump_action.IsPressed()) { /* ... */ }
        move_forward.GetValue();
    }
};
```

---

## 8. Plugin system (engine extensions)

Different from modules: a **plugin** extends the *engine* itself (e.g. an FBX importer), not a specific game's gameplay. Interface: `hn::IPlugin` (`src/plugins/iplugin.h`):

```cpp
virtual void Init(EngineInfos*);
virtual void Tick(double dt);
virtual void StartGame(); virtual void GameTick(double dt); virtual void EndGame();
// editor hooks:
virtual std::unordered_map<std::string, EditorWindow> HandleWindow();
virtual void HandleTitleBar();
virtual void HandleToolbar();
```

A plugin is exported with the `HORIZON_PLUGIN(PluginClass)` macro, which generates a `LoadPlugin()` entry point. The engine loads it via `hn::Private::SysPlugin`, scanning `Plugins/<category>/<name>/*.dll` at startup (see §2). A plugin must be compiled against the **same engine version**, otherwise it's rejected at load time.

Example included: `src-plugins/fbx-importer` — a skeleton FBX import plugin (`FBXImporter : IPlugin`), built and automatically copied into `Plugins/Import/FBXImporter/` by the CMake build.

---

## 9. Lua scripting

`hn::LuaVM` (`src/scripting/lua_vm.h`) embeds a Lua VM (headers under `third-party/lua`, statically linked into `horizon-next`).

- `RegisterHNStdFunctions()` — registers the engine's standard functions exposed to Lua (called automatically when the `Engine` is constructed).
- `RegisterFunction(name, LuaFunction)` — registers an additional C function.
- `DoFromFile(path)` — executes a Lua script.
- `Update(dt)` — called every frame by `HNFrameListener` to drive a global `Update` function on the scripting side.
- `GetScriptStack()` — stack of loaded scripts (useful for debugging/reloading).

---

## 10. Gameplay: input and multiplayer

- **`hge::InputAction`** / **`hge::InputAxis1D`** (`core/gameplay/input.h`) — binary actions (`IsPressed/IsHeld/IsReleased`) and continuous axes (`GetValue()`), mapped internally to keys (`Private::input_manager`).
- **Player Controllers** (`core/gameplay/player_controller.h`) — multiplayer handling: `CreatePlayer()/DeletePlayer(pc)`, `PossessActor(pc, actor)/UnpossessActor(pc)`, `GetPossessedActor(pc)`, per-player viewport (`SetPlayerViewportSize`, `GetPlayerViewportBackend`).
- On every simulation frame, `PlayerControllersTick(dt)` (a friend of `HGE_Actor`) internally calls `ProcessInput` on the actor possessed by each controller, before the physics update.

---

## 11. Logging

- **Global API** (`core/log/log.h`): `hn::Log(msg, ELog_type, print_to_viewport, viewport_time)` and `hn::LogViewport(msg, mode)`. `ELog_type` = `MSG | WARNING | ERR`.
- **Extensible class** `hn::Logger` (in `engine.h`): overridable (`Log()`, `RedirectMessages()`) and injectable via `Engine::SetCustomLoggerSubsystem`. The default logger prefixes `W:`/`ERR:` and can display the message on screen (viewport) through Ogre's `TrayManager`, auto-expiring after `viewport_time_` seconds.
- **Console redirection**: `EngineStreambuf` (intercepts `std::cout`) and `OgreLogRedirector` (redirects Ogre's internal logs into the engine's own system) — `core/log/redirect.h`.

---

## 12. Filesystem

`core/filesystem.h`: `InitFilesystem(useRawFiles)` toggles between raw files (editor mode) and reading from a packaged archive. `GetFileContent(path, &outSize, forceRawFile)` reads a file regardless of mode, `GetParentPath(path)` resolves the parent folder.

---

## 13. Rendering backend (Ogre3D)

Internal to `engine.cpp` (`struct rendering_interface`, not publicly exposed — reachable only through `Engine::GetRenderingBackend()` as `void*`, cast by components that need it). It wraps:
- `OgreBites::ApplicationContext` (window, app init)
- `Ogre::Root` + `Ogre::SceneManager`
- `Ogre::OverlaySystem` (UI overlay)
- `OgreBites::TrayManager` (on-screen debug messages)
- An `HNFrameListener` that drives the whole tick loop (see §2)

The rendering backends supported through Ogre (Direct3D11, OpenGL 3+, Vulkan) are listed in `api_friendly_names`, but the actual backend selection happens through Ogre's standard configuration, not a dedicated Horizon-Next API.

---

## 14. Build (CMake)

The `horizon-next` project (C11/C++20) builds:
- **`horizon-next`** — the engine DLL (SHARED), links `OgreMain` + `OgreBites`, bundles `inih` (ini parsing), `tinyxml2` (XML scenes), and Lua as third-party sources (`third-party/`).
- **`fbx-importer`** — the example plugin, copied into `Plugins/Import/FBXImporter/` next to the executable.
- **`example-game`** — an example gameplay module DLL (`game/src/`).
- **`game`** — the final executable (`src/main.cpp`), linked against `horizon-next`. Ogre's DLLs and media are copied next to the executable automatically as a post-build step.

The engine version is injected at compile time (`ENGINE_VERSION_STR="Horizon-Next 2026.1.0 beta"`), readable at runtime via `hn::GetEngineVersion()`.

⚠️ The `OGRE_DIR` path in `CMakeLists.txt` is currently hardcoded (`S:/Programmation/Libraries/OgreSDK/CMake`), as are the paths to the Ogre DLLs/Media copied in the post-build step — these need to be parameterized to build on another machine.

---

## 15. Minimal usage example

Excerpt from `src/main.cpp`:

```cpp
hn::Engine engine("default_engine.ini", /*editor=*/true, nullptr);
std::cout << hn::GetEngineVersion() << std::endl;

hn::Private::SysModule module("example-game.dll");
module.RegisterFactory();

int pc = hge::CreatePlayer();

hge::HGE_Level lvl{};
lvl.LoadFromFile("scene.xml");

engine.GetLuaVM()->DoFromFile("script.lua");

engine.StartMainLoop();

module.UnregisterFactory();
```

---

## 16. Points of attention / work in progress

- `src/core/data/data_table.h` is empty (not implemented).
- `src/core/data/data_enum.h` only contains commented-out code (a reflectable enum system, not active yet).
- `src/editor/main.cpp` is empty — the editor hasn't been started yet.
- Plugin/module loading (`.dll`) is only implemented for Windows (`#ifdef _WIN32`); the Linux/macOS branches are empty stubs.
- `HGE_Level`'s commented-out second constructor (`LoadFromFile` in the constructor) suggests an ongoing rework of the level-loading API.