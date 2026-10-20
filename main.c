#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <curl/curl.h>

// Helper function to clamp values within a specific range
double clamp(double val, double min_val, double max_val) {
    if (val < min_val) return min_val;
    if (val > max_val) return max_val;
    return val;
}

// Simple JSON Escape function for Pure C
void json_escape(const char *src, char *dest, size_t dest_size) {
    size_t j = 0;
    for (size_t i = 0; src[i] != '\0' && j < dest_size - 2; i++) {
        if (src[i] == '"') {
            dest[j++] = '\\';
            dest[j++] = '"';
        } else if (src[i] == '\\') {
            dest[j++] = '\\';
            dest[j++] = '\\';
        } else if (src[i] == '\n') {
            dest[j++] = '\\';
            dest[j++] = 'n';
        } else {
            dest[j++] = src[i];
        }
    }
    dest[j] = '\0';
}

// Helper to convert string to lowercase for case-insensitive keyword matching
void to_lowercase(const char *src, char *dest, size_t dest_size) {
    size_t i = 0;
    while (src[i] != '\0' && i < dest_size - 1) {
        dest[i] = tolower((unsigned char)src[i]);
        i++;
    }
    dest[i] = '\0';
}

// Dynamic Context Switching Analyzer:
// Evaluates the user prompt and dynamically shifts internal psychological/cognitive states
void update_dynamic_state(const char *user_prompt, double *logic_focus, double *relational_distance, double *agency_drive) {
    char lower_prompt[512];
    to_lowercase(user_prompt, lower_prompt, sizeof(lower_prompt));

    // Check for academic, technical, or analytical intent (High Logic Focus)
    if (strstr(lower_prompt, "analyze") || strstr(lower_prompt, "network") || 
        strstr(lower_prompt, "code") || strstr(lower_prompt, "system") || 
        strstr(lower_prompt, "error") || strstr(lower_prompt, "math")) {
        *logic_focus = 0.90;          // High logical rigor, low temperature
        *relational_distance = 0.30;  // Professional proximity
        *agency_drive = 0.75;         // Detailed response generation
    } 
    // Check for casual, playful, or chatting intent (Low Logic Focus, High Creativity/Randomness)
    else if (strstr(lower_prompt, "hello") || strstr(lower_prompt, "hi") || 
             strstr(lower_prompt, "fun") || strstr(lower_prompt, "chat") || 
             strstr(lower_prompt, "joke")) {
        *logic_focus = 0.20;          // Low logic, higher temperature for casual vibe
        *relational_distance = 0.70;  // More distant or casual boundary
        *agency_drive = 0.40;         // Shorter response length
    } 
    // Default / Neutral state if intent is ambiguous
    else {
        *logic_focus = 0.50;
        *relational_distance = 0.50;
        *agency_drive = 0.50;
    }
}

int main() {
    // Define initial internal state variables (Internal Cognitive State)
    double m_logic_focus = 0.5;
    double m_relational_distance = 0.5;
    double m_agency_drive = 0.5;

    // Simulate incoming user prompt (Can be switched dynamically)
    char *user_prompt = "Hello, analyze the current network state and system errors.";
    char *system_prompt = "You are a logical and cautious autonomous agent.";

    // Perform Dynamic Context Switching based on the user prompt content
    update_dynamic_state(user_prompt, &m_logic_focus, &m_relational_distance, &m_agency_drive);

    printf("=== Dynamic Context State Updated ===\n");
    printf("m_logic_focus        : %.2f\n", m_logic_focus);
    printf("m_relational_distance: %.2f\n", m_relational_distance);
    printf("m_agency_drive       : %.2f\n", m_agency_drive);

    // --- Calculate Hyperparameters using mathematical models ---
    double T = fmax(0.05, 0.9 - 0.6 * m_logic_focus);
    double P_top_p = fmax(0.10, 0.95 - 0.25 * m_logic_focus);
    double P_presence = 0.1 + 0.4 * m_relational_distance;
    double P_frequency = 0.1 + 0.3 * m_relational_distance;
    int N_tokens = (int)fmax(32, floor(128 + 384 * m_agency_drive));

    printf("\n=== Calculated Hyperparameters ===\n");
    printf("Temperature (T)   : %.4f\n", T);
    printf("Top-p             : %.4f\n", P_top_p);
    printf("Presence Penalty  : %.4f\n", P_presence);
    printf("Frequency Penalty : %.4f\n", P_frequency);
    printf("Max Tokens        : %d\n", N_tokens);

    // Escape strings for safe JSON payload formatting
    char escaped_system[512];
    char escaped_user[512];
    json_escape(system_prompt, escaped_system, sizeof(escaped_system));
    json_escape(user_prompt, escaped_user, sizeof(escaped_user));

    // Construct JSON Payload
    char payload[2048];
    snprintf(payload, sizeof(payload),
        "{\"system_prompt\": \"%s\", \"user_prompt\": \"%s\", \"temperature\": %.4f, \"top_p\": %.4f, \"presence_penalty\": %.4f, \"frequency_penalty\": %.4f, \"max_tokens\": %d, \"seed\": 42}",
        escaped_system, escaped_user, T, P_top_p, P_presence, P_frequency, N_tokens
    );

    printf("\n=== Generated JSON Payload ===\n%s\n", payload);

    // Simulate sending HTTP POST request to TokenMe API using libcurl
    CURL *curl = curl_easy_init();
    if(curl) {
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        // headers = curl_slist_append(headers, "Authorization: Bearer YOUR_API_KEY");

        // Set TokenMe API endpoint
        curl_easy_setopt(curl, CURLOPT_URL, "https://api.tokenme.limited/v1/inference");
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // Uncomment below lines to execute real HTTP request
        // CURLcode res = curl_easy_perform(curl);
        // if(res != CURLE_OK) fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_stderror(res));

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
        printf("\n[Info] libcurl initialized and payload structured successfully with dynamic context.\n");
    }

    return 0;
}