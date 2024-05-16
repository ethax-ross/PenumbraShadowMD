/*
    Copyright 2021-2023 Picovoice Inc.

    You may not use this file except in compliance with the license. A copy of the license is located in the "LICENSE"
    file accompanying this source.

    Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on
    an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the
    specific language governing permissions and limitations under the License.
*/

/**
 * UUID d8 84 7d bf 69 52 29 1c
**/

// Can only use Serial OR Serial1 (not both at the same time!)
// #define SERIAL Serial
#define SERIAL Serial1
#define DEBUG Serial

#include <Picovoice_EN.h>

#include "R2_en_cortexm_v3_0_0.h"
#include "pv_porcupine_params.h"

#define MEMORY_BUFFER_SIZE (70 * 1024)

static const char* ACCESS_KEY = "TODO";

static pv_picovoice_t *handle = NULL;

static int8_t memory_buffer[MEMORY_BUFFER_SIZE] __attribute__((aligned(16)));

static const float PORCUPINE_SENSITIVITY = 0.75f;
static const float RHINO_SENSITIVITY = 0.5f;
static const float RHINO_ENDPOINT_DURATION_SEC = 1.0f;
static const bool RHINO_REQUIRE_ENDPOINT = true;

static void wake_word_callback(void) {
    DEBUG.println("Wake word detected!");
    SERIAL.println("R2_LISTENING\r");
}

static void inference_callback(pv_inference_t *inference) {
    char *state = nullptr;
    char *panel = nullptr;
    char *sequence = nullptr;
    char *tool = nullptr;

    DEBUG.println("{");
    DEBUG.print("    is_understood : ");
    DEBUG.println(inference->is_understood ? "true" : "false");
    if (inference->is_understood) {
      DEBUG.print("    intent : ");
      DEBUG.println(inference->intent);
      if (inference->num_slots > 0) {
        DEBUG.println("    slots : {");
        for (int32_t i = 0; i < inference->num_slots; i++) {
          DEBUG.print("        ");
          DEBUG.print(inference->slots[i]);
          DEBUG.print(" : ");
          DEBUG.println(inference->values[i]);

          if (strcmp(inference->slots[i], "state") == 0) {
            state = (char *)malloc(strlen(inference->values[i]) + 1);
            if (state != nullptr) {
              strcpy(state, inference->values[i]);
            }
          } else if (strcmp(inference->slots[i], "panel") == 0) {
            panel = (char *)malloc(strlen(inference->values[i]) + 1);
            if (panel != nullptr) {
              strcpy(panel, inference->values[i]);
            }
          } else if (strcmp(inference->slots[i], "sequence") == 0) {
            sequence = (char *)malloc(strlen(inference->values[i]) + 1);
            if (sequence != nullptr) {
              strcpy(sequence, inference->values[i]);
            }
          } else if (strcmp(inference->slots[i], "tool") == 0) {
            tool = (char *)malloc(strlen(inference->values[i]) + 1);
            if (tool != nullptr) {
              strcpy(tool, inference->values[i]);
            }
          } 
        }

        DEBUG.println("    }");
      }

      if(inference->intent == String("controlPanels")) {
        control_panel(state, panel);
      }
      
      if(inference->intent == String("playSequence")) {
        play_sequence(sequence);
      }

      if(inference->intent == String("useTool")) {
        use_tool(tool);
      }
    }

    DEBUG.println("}\n");

    pv_inference_delete(inference);
}

void play_sequence(const char* sequence) {
  DEBUG.println("Sequence: " + String(sequence));

  if (String(sequence) == "run" || String(sequence) == "scream" || String(sequence) == "the empire are coming") {
    Serial.println("#87,#79,#80,#78\r");
  }

  if (strstr(sequence, "message") != NULL) {
    Serial.println("#9\r");
  }

  if (String(sequence) == "cantina") {
    Serial.println("#10\r");
  }

  if (String(sequence) == "marching ants") {
    Serial.println("#19\r");
  }

  if (String(sequence) == "wave" || strstr(sequence, "bye") != NULL) {
    Serial.println("#81,MP3=42\r");
  }

  if (String(sequence) == "knight rider" || strstr(sequence, "kitt") != NULL) {
    Serial.println("KNIGHT_RIDER\r");
  }

  if (String(sequence) == "disco" || String(sequence) == "party") {
    Serial.println("#8\r");
  }

  if (String(sequence) == "smirk") {
    Serial.println("#7\r");
  }
}

void use_tool(const char* tool) {
  DEBUG.println("Use tool: " + String(tool));

  if (strstr(tool, "gripper") != NULL) {
    Serial.println("#84\r");
  }

  if (strstr(tool, "interface") != NULL) {
    Serial.println("#85\r");
  }
}

void control_panel(const char* state, const char* panel) {
  DEBUG.println("Control panel: " + String(state) + " " + String(panel));
  
  if (String(panel) == "panels") {
    String(state) == "open" ? Serial.println("#30,#62,#68,#74,#56,#58,#60\r") : Serial.println("#33,#63,#69,#75,#57,#59,#61\r");
  }

  if (String(panel) == "body panels") {
    String(state) == "open" ? Serial.println("#62,#68,#74,#56\r") : Serial.println("#63,#69,#75,#57\r");
  }

  if (String(panel) == "dome panels") {
    String(state) == "open" ? Serial.println("#30\r") : Serial.println("#33\r");
  }

  if (String(panel) == "pie panels") {
    String(state) == "open" ? Serial.println("#46,#48,#50,#52\r") : Serial.println("#47,#49,#51,#53\r");
  }

  if (String(panel) == "tool doors") {
    String(state) == "open" ? Serial.println("#62,#68\r") : Serial.println("#63,#69\r");
  }

  if (strstr(panel, "charge bay") != NULL) {
    String(state) == "open" ? Serial.println("#74\r") : Serial.println("#75\r");
  }

  if (strstr(panel, "data port") != NULL) {
    String(state) == "open" ? Serial.println("#56\r") : Serial.println("#57\r");
  }

  if (String(panel) == "utility arms") {
    Serial.println("#82\r");
  }
}

static void print_error_message(char **message_stack, int32_t message_stack_depth) {
    for (int32_t i = 0; i < message_stack_depth; i++) {
        DEBUG.println(message_stack[i]);
    }
}

void setup() {
    SERIAL.begin(9600);
    while (!SERIAL);

    pv_status_t status = pv_audio_rec_init();
    if (status != PV_STATUS_SUCCESS) {
        DEBUG.print("Audio init failed with ");
        DEBUG.println(pv_status_to_string(status));
        while (1);
    }

    char **message_stack = NULL;
    int32_t message_stack_depth = 0;
    pv_status_t error_status;

    status = pv_picovoice_init(
        ACCESS_KEY,
        MEMORY_BUFFER_SIZE,
        memory_buffer,
        sizeof(KEYWORD_ARRAY),
        KEYWORD_ARRAY,
        PORCUPINE_SENSITIVITY,
        wake_word_callback,
        sizeof(CONTEXT_ARRAY),
        CONTEXT_ARRAY,
        RHINO_SENSITIVITY,
        RHINO_ENDPOINT_DURATION_SEC,
        RHINO_REQUIRE_ENDPOINT,
        inference_callback,
        &handle);
    if (status != PV_STATUS_SUCCESS) {
        DEBUG.print("Picovoice init failed with ");
        DEBUG.println(pv_status_to_string(status));

        error_status = pv_get_error_stack(&message_stack, &message_stack_depth);
        if (error_status != PV_STATUS_SUCCESS) {
            DEBUG.println("Unable to get Porcupine error state");
            while (1);
        }
        print_error_message(message_stack, message_stack_depth);
        pv_free_error_stack(message_stack);

        while (1);
    }

    const char *rhino_context = NULL;
    status = pv_picovoice_context_info(handle, &rhino_context);
    if (status != PV_STATUS_SUCCESS) {
        DEBUG.print("retrieving context info failed with");
        DEBUG.println(pv_status_to_string(status));
        while (1);
    }
    DEBUG.println("Wake word: 'R2'");
    DEBUG.println(rhino_context);
}

void loop() {
    const int16_t *buffer = pv_audio_rec_get_new_buffer();
    if (buffer) {
        const pv_status_t status = pv_picovoice_process(handle, buffer);
        if (status != PV_STATUS_SUCCESS) {
            DEBUG.print("Picovoice process failed with ");
            DEBUG.println(pv_status_to_string(status));
            char **message_stack = NULL;
            int32_t message_stack_depth = 0;
            pv_get_error_stack(
                &message_stack,
                &message_stack_depth);
            for (int32_t i = 0; i < message_stack_depth; i++) {
                DEBUG.println(message_stack[i]);
            }
            pv_free_error_stack(message_stack);
            while (1);
        }
    }
}
