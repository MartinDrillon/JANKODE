---
applyTo: '**'
---
Provide project context and coding guidelines that AI should follow when generating code, answering questions, or reviewing changes.

1. **Project Context**:
   - The project is a MIDI controller firmware for a custom hardware device.
   - It involves reading analog inputs, processing MIDI messages, and managing a key matrix.
   - The code is structured in a modular way, with separate files for different functionalities (e.g., scanning, key mapping, MIDI handling).

2. **Coding Guidelines**:
   - Follow consistent naming conventions (e.g., camelCase for variables, PascalCase for classes).
   - Use comments to explain complex logic or important decisions.
   - Keep functions small and focused on a single task.
   - Avoid global variables when possible; use function parameters and return values to pass data.
   - Include error handling and input validation where appropriate.
   - Optimize for performance, especially in time-critical sections (e.g., ADC readings, MIDI processing).
   - Write unit tests for critical functions to ensure reliability.