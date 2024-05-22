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
// #define MARCDUINO Serial
#define MARCDUINO Serial1
#define DEBUG_ENABLED false

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

void DEBUG(String message) {
  if (DEBUG_ENABLED) {
    Serial.println(message);
  }
}

static void wake_word_callback(void) {
    DEBUG("Wake word detected!");
    // turn on front holo for 'listening'
    MARCDUINO.println("#96\r");
}

static void inference_callback(pv_inference_t *inference) {
    char *state = nullptr;
    char *panel = nullptr;
    char *sequence = nullptr;
    char *tool = nullptr;

    DEBUG("{");
    DEBUG("    is_understood : ");
    DEBUG(inference->is_understood ? "true" : "false");
    if (inference->is_understood) {
      DEBUG("    intent : ");
      DEBUG(inference->intent);
      if (inference->num_slots > 0) {
        DEBUG("    slots : {");
        for (int32_t i = 0; i < inference->num_slots; i++) {
          DEBUG("        ");
          DEBUG(inference->slots[i]);
          DEBUG(" : ");
          DEBUG(inference->values[i]);

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

        DEBUG("    }");
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

    DEBUG("}\n");

    pv_inference_delete(inference);
}

void send_command(String cmd) {
  // turn off front holo for 'stop listening' and send command as well
  MARCDUINO.println("#97," + cmd + "\r");
}

void play_sequence(const char* sequence) {
  DEBUG("Sequence: " + String(sequence));

  if (String(sequence) == "run" || String(sequence) == "scream" || String(sequence) == "the empire are coming") {
    send_command("#87,#79,#80,#78");
  }

  if (strstr(sequence, "stop") != NULL) {
    send_command("RESET");
  }

  if (strstr(sequence, "message") != NULL) {
    send_command("#9");
  }

  if (String(sequence) == "cantina") {
    send_command("#10");
  }

  if (String(sequence) == "marching ants") {
    send_command("#19");
  }

  if (String(sequence) == "wave" || strstr(sequence, "bye") != NULL) {
    send_command("#81,MP3=42");
  }

  if (String(sequence) == "knight rider" || strstr(sequence, "kitt") != NULL) {
    send_command("KNIGHT_RIDER");
  }

  if (String(sequence) == "disco" || String(sequence) == "party") {
    send_command("#8");
  }

  if (String(sequence) == "smirk") {
    send_command("#7");
  }
}

void use_tool(const char* tool) {
  DEBUG("Use tool: " + String(tool));

  if (strstr(tool, "gripper") != NULL) {
    send_command("#84");
  }

  if (strstr(tool, "interface") != NULL) {
    send_command("#85");
  }
}

void control_panel(const char* state, const char* panel) {
  DEBUG("Control panel: " + String(state) + " " + String(panel));
  
  if (String(panel) == "panels") {
    String(state) == "open" ? send_command("#30,#62,#68,#74,#56,#58,#60") : send_command("#33,#63,#69,#75,#57,#59,#61");
  }

  if (String(panel) == "body panels") {
    String(state) == "open" ? send_command("#62,#68,#74,#56") : send_command("#63,#69,#75,#57");
  }

  if (String(panel) == "dome panels") {
    String(state) == "open" ? send_command("#30") : send_command("#33");
  }

  if (String(panel) == "pie panels") {
    String(state) == "open" ? send_command("#46,#48,#50,#52") : send_command("#47,#49,#51,#53");
  }

  if (String(panel) == "tool doors") {
    String(state) == "open" ? send_command("#62,#68") : send_command("#63,#69");
  }

  if (strstr(panel, "charge bay") != NULL) {
    String(state) == "open" ? send_command("#74") : send_command("#75");
  }

  if (strstr(panel, "data port") != NULL) {
    String(state) == "open" ? send_command("#56") : send_command("#57");
  }

  if (String(panel) == "utility arms") {
    send_command("#82");
  }
}

static void print_error_message(char **message_stack, int32_t message_stack_depth) {
    for (int32_t i = 0; i < message_stack_depth; i++) {
        DEBUG(message_stack[i]);
    }
}

void setup() {
    MARCDUINO.begin(9600);
    while (!MARCDUINO);

    pv_status_t status = pv_audio_rec_init();
    if (status != PV_STATUS_SUCCESS) {
        DEBUG("Audio init failed with ");
        DEBUG(pv_status_to_string(status));
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
        DEBUG("Picovoice init failed with ");
        DEBUG(pv_status_to_string(status));

        error_status = pv_get_error_stack(&message_stack, &message_stack_depth);
        if (error_status != PV_STATUS_SUCCESS) {
            DEBUG("Unable to get Porcupine error state");
            while (1);
        }
        print_error_message(message_stack, message_stack_depth);
        pv_free_error_stack(message_stack);

        while (1);
    }

    const char *rhino_context = NULL;
    status = pv_picovoice_context_info(handle, &rhino_context);
    if (status != PV_STATUS_SUCCESS) {
        DEBUG("retrieving context info failed with");
        DEBUG(pv_status_to_string(status));
        while (1);
    }
    DEBUG("Wake word: 'R2'");
    DEBUG(rhino_context);
}

void loop() {
    const int16_t *buffer = pv_audio_rec_get_new_buffer();
    if (buffer) {
        const pv_status_t status = pv_picovoice_process(handle, buffer);
        if (status != PV_STATUS_SUCCESS) {
            DEBUG("Picovoice process failed with ");
            DEBUG(pv_status_to_string(status));
            char **message_stack = NULL;
            int32_t message_stack_depth = 0;
            pv_get_error_stack(
                &message_stack,
                &message_stack_depth);
            for (int32_t i = 0; i < message_stack_depth; i++) {
                DEBUG(message_stack[i]);
            }
            pv_free_error_stack(message_stack);
            while (1);
        }
    }
}
