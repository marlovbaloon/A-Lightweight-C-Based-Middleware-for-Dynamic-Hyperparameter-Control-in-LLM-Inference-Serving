# A Lightweight C-Based Middleware for Dynamic Hyperparameter Control in LLM Inference Serving

A high-performance, deterministic, zero-dependency C middleware designed to dynamically map cognitive and relational intent states into optimal LLM sampling hyperparameters (T, Top-p, Penalties) in real-time before reaching the inference engine.

---

## 1. Problem Statement

Controlling Large Language Model (LLM) behavior dynamically during inference serving currently suffers from three critical bottlenecks:

1. **Prompt Engineering Instability (Soft Constraints):** Instructing models via system prompts (e.g., "be concise and analytical") yields non-deterministic outputs and remains vulnerable to hallucinations because prompt instructions do not strictly enforce mathematical bounds at the sampling layer.
2. **Framework Bloat & Runtime Overhead:** Existing agentic frameworks (e.g., Python-based abstractions like LangChain) introduce heavy runtime latencies, high memory footprints, and non-deterministic overheads, making them unsuitable for resource-constrained Edge environments or low-latency gateway architectures.
3. **Lack of Quantitative State-to-Sampling Mapping:** Current systems lack a unified mathematical formulation to deterministically compute dynamic hyperparameters straight from contextual or emotional state representations.

---

## 2. Core Contributions & System Architecture

This project introduces a bare-metal compatible C middleware that addresses these limitations through three foundational innovations:

* **Quantitative Psycho-Semantic Mapping:** A deterministic mathematical layer that computes safe sampling bounds directly from an internal cognitive state vector M.
* **Zero-Dependency Pure C Runtime:** Built with pure standard C and libcurl, achieving ultra-low microsecond execution latencies and minimal memory footprint suitable for embedding in front of GPU fleets (e.g., vLLM, TensorRT-LLM, TokenMe API).
* **Configuration-Driven Dynamic Context Switching:** Externalized JSON keyword mappings (Klogic, Kcasual) that allow intent-based context updates within a single request pass without recompilation.

---

## 3. Formal Semantics & Mathematical Model

### System Axioms
* **Axiom 1 (State Boundedness):** Every component of the internal state vector M is strictly bounded within a closed interval: m in [0.0, 1.0].
* **Axiom 2 (Deterministic Execution):** Context evaluation and parameter transformation function with linear time complexity O(N) with respect to string stream length, without stochastic variation.

### Internal Cognitive Vector & Intent Mapping
Let the agent's internal cognitive state vector be defined as:
M = (mLogicFocus, mRelationalDistance, mAgencyDrive)

Given external keyword sets Klogic and Kcasual loaded from external JSON configurations, the dynamic state transition function updateDynamicState(Puser) maps user prompt Puser to state vector M:

* Klogic = {"analyze", "network", "code", "system", "error", "math"}
* Kcasual = {"hello", "hi", "fun", "chat", "joke"}

updateDynamicState(Puser) =
- (0.90, 0.30, 0.75) if Puser intersects with Klogic is not empty
- (0.20, 0.70, 0.40) if Puser intersects with Kcasual is not empty
- (0.50, 0.50, 0.50) otherwise (Neutral State)

### Hyperparameter Derivation Formulation
The derived cognitive state M is directly converted into explicit LLM sampling controls:

1. **Temperature (T):**
   T = max(0.05, 0.9 - 0.6 * mLogicFocus)
2. **Top-p (Ptopp):**
   Ptopp = max(0.10, 0.95 - 0.25 * mLogicFocus)
3. **Presence Penalty (Ppresence):**
   Ppresence = 0.1 + 0.4 * mRelationalDistance
4. **Frequency Penalty (Pfrequency):**
   Pfrequency = 0.1 + 0.3 * mRelationalDistance
5. **Maximum Tokens (Ntokens):**
   Ntokens = max(32, floor(128 + 384 * mAgencyDrive))

---

## 4. Theoretical Invariant & Formal Proof

### Theorem 1 (Hyperparameter Safe Boundary Invariant)
For all internal logic states mLogicFocus in [0.0, 1.0], the calculated temperature T is mathematically guaranteed to remain within safe sampling bounds T in [0.05, 0.90].

#### Proof:
Consider the lower and upper bounds of T under the bounded domain mLogicFocus in [0.0, 1.0]:
1. **Maximum Logic Focus** (mLogicFocus = 1.0): 
   T = max(0.05, 0.9 - 0.6 * 1.0) = max(0.05, 0.30) = 0.30
2. **Minimum Logic Focus** (mLogicFocus = 0.0): 
   T = max(0.05, 0.9 - 0.6 * 0.0) = max(0.05, 0.90) = 0.90

Since T is lower-bounded by max(0.05, ...), T >= 0.05 holds for all valid state inputs. Hence, T in [0.05, 0.90]. Q.E.D.

---

## 5. Proof-of-Concept Execution Output

```text
=== Dynamic Context State Updated ===
m_logic_focus        : 0.90
m_relational_distance: 0.30
m_agency_drive       : 0.75

=== Calculated Hyperparameters ===
Temperature (T)   : 0.3600
Top-p             : 0.7250
Presence Penalty  : 0.2200
Frequency Penalty : 0.1900
Max Tokens        : 416

=== Generated JSON Payload ===
{"system_prompt": "You are a logical and cautious autonomous agent.", "user_prompt": "Hello, analyze the current network state and system errors.", "temperature": 0.3600, "top_p": 0.7250, "presence_penalty": 0.2200, "frequency_penalty": 0.1900, "max_tokens": 416, "seed": 42}

[Info] libcurl initialized and payload structured successfully with dynamic context.

```

---

## 6. Build and Execution Instructions

### Prerequisites

* GCC compiler (C11 support)
* libcurl development headers (libcurl4-openssl-dev on Ubuntu/Debian)
* make build utility

### Building from Source

```bash
# Clone repository
git clone [https://github.com/marlobaloon/A-Lightweight-C-Based-Middleware-for-Dynamic-Hyperparameter-Control-in-LLM-Inference-Serving.git](https://github.com/marlobaloon/A-Lightweight-C-Based-Middleware-for-Dynamic-Hyperparameter-Control-in-LLM-Inference-Serving.git)
cd A-Lightweight-C-Based-Middleware-for-Dynamic-Hyperparameter-Control-in-LLM-Inference-Serving

# Compile executable
gcc -Wall -Wextra -O2 -o llm_bridge main.c -lcurl -lm

# Run middleware executable
./llm_bridge

```

---

## 7. Limitations & Future Work

This codebase serves as an architectural Proof-of-Concept (PoC) demonstrating formal mathematical transformation and low-overhead C execution.

Future milestones include:

* Conducting empirical benchmark evaluations against real GPU LLM inference serving instances.
* Measuring latency, throughput, and accuracy metrics comparing dynamic hyperparameter adaptation against static sampling baseline models.
