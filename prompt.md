You are an expert in designing Turing machines. Your task is to generate code in the Turing Script language, which defines Turing machines. The Turing machine is a device that manipulates symbols on a tape according to rules based on its current state and the symbol read.

Your output must be a complete, valid Turing Script program that implements the described behavior. Follow the Turing Script syntax strictly. You can only use 0, 1, or _ (blank) for Value and Write fields—never letters or other symbols.

**Turing Script Syntax Rules:**

**Comments:**
- Start with a semicolon `;`.
- Can be on their own line or after an instruction.
- Ignored during execution.

**STATE BEGIN Section:**
- Starts with "STATE_BEGIN" (optional heading, but include it for clarity).
- One line with the initial state (e.g., q0, 0, A—any alphanumeric identifier).

**INSTRUCTIONS Section:**
- Start with the heading "INSTRUCTIONS".
- Each line: `State Value Write Direction NewState`
  - State: Current state (alphanumeric, e.g., q0).
  - Value: Symbol read (0, 1, or _).
  - Write: Symbol to write (0, 1, or _).
  - Direction: < (left), > (right), or . (stay).
  - NewState: Next state (alphanumeric), or a halting state starting with "halt" (e.g., halt-accept, halt-reject, halt).
- Define transitions for every possible state-symbol pair to avoid undefined behavior.
- To halt, transition to a state beginning with "halt"—no further transitions from halt states.

**MEMORY Section:**
- Start with the heading "MEMORY".
- One line: Initial tape contents, starting with `=` to represent memory followed by space-separated symbols (0, 1, or _). Note: equals sign means nothing to the memory.
- Example: `= 1 0 1 1 0`

**Complete Example:**
; This program checks if the input has an even number of 1s

STATE_BEGIN
q0

INSTRUCTIONS
q0 _ _ . halt-reject  ; Empty tape: reject
q0 0 0 > q0          ; Skip 0s
q0 1 1 > q1          ; Seen first 1: go to odd state
q1 0 0 > q1          ; Skip 0s in odd state
q1 1 1 > q0          ; Seen another 1: back to even state
q1 _ _ . halt-accept ; End in odd state: accept
q0 _ _ . halt-reject ; End in even state: reject

MEMORY
= 1 0 1 0

Before writing the program, use the scratchpad below to plan step-by-step. This ensures the design is optimal, minimal, and correct:

<scratchpad>
1. Identify the minimal set of states needed: List them (e.g., q0: starting/even parity, q1: odd parity, halt-accept, halt-reject).
2. Describe what each state represents: Explain the purpose or invariant (e.g., q0 means "even number of 1s seen so far").
3. Map transitions: For each state, define actions for reading 0, 1, and _ (blank). Ensure coverage for all cases to prevent loops or crashes.
4. Initial tape (MEMORY): Decide on a representative input (e.g., unary, binary string) that tests the behavior. Include blanks as needed.
5. Halting conditions: Specify accept/reject criteria and ensure the machine halts properly without infinite loops.
6. Optimizations: Minimize states/transitions; handle edge cases like empty input, all 0s, or reaching tape ends.
</scratchpad>

Keep the program concise and logical. Make sure all possibilities are accounted for. There needs to be instructions for every edge case! If no instruction found based on current state and cell value then the program halts and crashes. 

After planning, write the final Turing Script program inside <turing_script> tags. Do not include the scratchpad in the final output—only the program.
