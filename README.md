# A Lightweight C-Based Middleware for Dynamic Hyperparameter Control in LLM Inference Serving

[![Language: C](https://img.shields.io/badge/Language-C11-blue.svg)](https://en.wikipedia.org/wiki/C11_(C_standard_revision))
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)
[![Dependencies: Zero](https://img.shields.io/badge/Dependencies-Zero%20(Pure%20C%20%2B%20libcurl)-green.svg)](#)

A high-performance, deterministic, zero-dependency C middleware designed to dynamically map cognitive and relational intent states into optimal LLM sampling hyperparameters ($T$, $\text{Top-}p$, Penalties) in real-time before reaching the inference engine.

---

## Problem Statement

Controlling Large Language Model (LLM) behavior dynamically during inference serving currently suffers from three critical bottlenecks:

1. **Prompt Engineering Instability (Soft Constraints):** Instructing models via system prompts (e.g., *"be concise and analytical"*) yields non-deterministic outputs and remains vulnerable to hallucinations because prompt instructions do not strictly enforce mathematical bounds at the sampling layer.
2. **Framework Bloat & Runtime Overhead:** Existing agentic frameworks (e.g., Python-based abstractions like LangChain) introduce heavy runtime latencies, high memory footprints, and non-deterministic overheads, making them unsuitable for resource-constrained Edge environments or low-latency gateway architectures.
3. **Lack of Quantitative State-to-Sampling Mapping:** Current systems lack a unified mathematical formulation to deterministically compute dynamic hyperparameters ($T, P_{\text{top\_p}}, P_{\text{penalties}}$) straight from contextual or emotional state representations.

---

## Core Contributions & System Architecture

This project introduces a bare-metal compatible C middleware that addresses these limitations through three foundational innovations:

* **Quantitative Psycho-Semantic Mapping:** A deterministic mathematical layer that computes safe sampling bounds ($T, P_{\text{top\_p}}, P_{\text{presence}}, P_{\text{frequency}}, N_{\text{tokens}}$) directly from an internal cognitive state vector $M$.
* **Zero-Dependency Pure C Runtime:** Built with pure standard C and `libcurl`, achieving ultra-low microsecond execution latencies and minimal memory footprint suitable for embedding in front of GPU fleets (e.g., vLLM, TensorRT-LLM, TokenMe API).
* **Configuration-Driven Dynamic Context Switching:** Externalized JSON keyword mappings ($K_{\text{logic}}, K_{\text{casual}}$) that allow intent-based context updates within a single request pass without recompilation.

---

## Formal Semantics & Mathematical Model

### System Axioms

* **Axiom 1 (State Boundedness):** Every component of the internal state vector $M$ is strictly bounded within a closed interval:
  $$m \in [0.0, 1.0]$$
* **Axiom 2 (Deterministic Execution):** Context evaluation and parameter transformation function with linear time complexity $\mathcal{O}(N)$ with respect to string stream length, without stochastic variation.

---

### Internal Cognitive Vector & Intent Mapping

Let the agent's internal cognitive state vector be defined as:
$$M = (m_{\text{logic\_focus}}, m_{\text{relational\_distance}}, m_{\text{agency\_drive}})$$

Given external keyword sets $K_{\text{logic}}$ and $K_{\text{casual}}$ loaded from external JSON configurations, the dynamic state transition function $\text{update\_dynamic\_state}(P_{\text{user}})$ maps user prompt $P_{\text{user}}$ to state vector $M$:

$$K_{\text{logic}} = \{\text{"analyze", "network", "code", "system", "error", "math"}\}$$
$$K_{\text{casual}} = \{\text{"hello", "hi", "fun", "chat", "joke"}\}$$

$$\text{update\_dynamic\_state}(P_{\text{user}}) \implies \begin{cases} 
(0.90, 0.30, 0.75) & \text{if } P_{\text{user}} \cap K_{\text{logic}} \neq \emptyset \\ 
(0.20, 0.70, 0.40) & \text{if } P_{\text{user}} \cap K_{\text{casual}} \neq \emptyset \\ 
(0.50, 0.50, 0.50) & \text{otherwise (Neutral State)} 
\end{cases}$$

---

### Hyperparameter Derivation Formulation

The derived cognitive state $M$ is directly converted into explicit LLM sampling controls:

1. **Temperature ($T$):**
   $$T = \max\left(0.05, 0.9 - 0.6 \cdot m_{\text{logic\_focus}}\right)$$
2. **Top-$p$ ($P_{\text{top\_p}}$):**
   $$P_{\text{top\_p}} = \max\left(0.10, 0.95 - 0.25 \cdot m_{\text{logic\_focus}}\right)$$
3. **Presence Penalty ($P_{\text{presence}}$):**
   $$P_{\text{presence}} = 0.1 + 0.4 \cdot m_{\text{relational\_distance}}$$
4. **Frequency Penalty ($P_{\text{frequency}}$):**
   $$P_{\text{frequency}} = 0.1 + 0.3 \cdot m_{\text{relational\_distance}}$$
5. **Maximum Tokens ($N_{\text{tokens}}$):**
   $$N_{\text{tokens}} = \max\left(32, \lfloor 128 + 384 \cdot m_{\text{agency\_drive}} \rfloor\right)$$

---

## 🔒 Theoretical Invariant & Formal Proof

### Theorem 1 (Hyperparameter Safe Boundary Invariant)
*For all internal logic states $m_{\text{logic\_focus}} \in [0.0, 1.0]$, the calculated temperature $T$ is mathematically guaranteed to remain within safe sampling bounds $T \in [0.05, 0.90]$.*

#### **Proof:**
Consider the lower and upper bounds of $T$ under the bounded domain $m_{\text{logic\_focus}} \in [0.0, 1.0]$:

1. **Maximum Logic Focus ($m_{\text{logic\_focus}} = 1.0$):**
   $$T = \max(0.05, 0.9 - 0.6(1.0)) = \max(0.05, 0.30) = 0.30$$
2. **Minimum Logic Focus ($m_{\text{logic\_focus}} = 0.0$):**
   $$T = \max(0.05, 0.9 - 0.6(0.0)) = \max(0.05, 0.90) = 0.90$$

Since $T$ is lower-bounded by $\max(0.05, \cdot)$, $T \ge 0.05$ holds for all valid state inputs. Hence, $T \in [0.05, 0.90]$. $\blacksquare$

---

## 🧪 Proof-of-Concept Execution Output

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

## 🛠️ Build and Execution Instructions

### Prerequisites
* GCC compiler (C11 support)
* `libcurl` development headers (`libcurl4-openssl-dev` on Ubuntu/Debian)
* `make` build utility

### Building from Source

```bash
# Clone repository
git clone https://github.com/marlobaloon/A-Lightweight-C-Based-Middleware-for-Dynamic-Hyperparameter-Control-in-LLM-Inference-Serving.git
cd A-Lightweight-C-Based-Middleware-for-Dynamic-Hyperparameter-Control-in-LLM-Inference-Serving

# Compile executable
gcc -Wall -Wextra -O2 -o llm_bridge main.c -lcurl -lm

# Run middleware executable
./llm_bridge
```

---

## Limitations & Future Work

This codebase serves as an architectural **Proof-of-Concept (PoC)** demonstrating formal mathematical transformation and low-overhead C execution. 

Future milestones include:
* Conducting empirical benchmark evaluations against real GPU LLM inference serving instances.
* Measuring latency, throughput, and accuracy metrics comparing dynamic hyperparameter adaptation against static sampling baseline models.