#include <stdio.h>
#include <stdlib.h>
#include <GLFW/glfw3.h>

GLFWwindow* window;

typedef struct Game_State {
	float l_pos;
  float l_size;
  float l_speed;
	float r_pos;
  float r_size;
  float r_speed;
	float ball_x, ball_y;
	float ball_velx, ball_vely;
	int screen_w, screen_h;
} Game_State;

char w_down = 0;
char s_down = 0;
char i_down = 0;
char k_down = 0;

void key_callback(GLFWwindow* w, int key, int scancode, int action, int mods) {
  if (action == GLFW_PRESS && key == GLFW_KEY_ESCAPE) {
    glfwSetWindowShouldClose(window, 1);
  }
  if (action == GLFW_PRESS) {
    if (key == GLFW_KEY_W) {
      w_down = 1;
    }
    if (key == GLFW_KEY_S) {
      s_down = 1;
    }
    if (key == GLFW_KEY_I) {
      i_down = 1;
    }
    if (key == GLFW_KEY_K) {
      k_down = 1;
    }
  }
  else if (action == GLFW_RELEASE) {
    if (key == GLFW_KEY_W) {
      w_down = 0;
    }
    if (key == GLFW_KEY_S) {
      s_down = 0;
    }
    if (key == GLFW_KEY_I) {
      i_down = 0;
    }
    if (key == GLFW_KEY_K) {
      k_down = 0;
    }
  }
}


/** Initialise the game state */
void init_state(Game_State* state, int screen_w, int screen_h) {
  state->screen_w = screen_w;
  state->screen_h = screen_h;
  state->l_size = 128;
  state->r_size = 128;
  state->l_pos = screen_h/2 - state->l_size/2;
  state->r_pos = screen_h/2 - state->r_size/2;
  state->l_speed = 0;
  state->r_speed = 0;
  state->ball_x = screen_w / 2;
  state->ball_y = screen_h / 2;
  state->ball_velx = 4;
  state->ball_vely = 4;
}

/**
 * Initialise GLFW, create a window and set input callbacks
 */
GLFWwindow* init_glfw(Game_State* s) {
  glfwInit();
  window = glfwCreateWindow(s->screen_w, s->screen_h, "Pong", NULL, NULL);
  glfwMakeContextCurrent(window);
  glfwSetKeyCallback(window, key_callback);
  return window;
}

/** Initialise GL, set up projection matrices and viewport */
void init_gl(Game_State* s) {
  glClearColor(0, 0, 0, 1);
  glMatrixMode(GL_PROJECTION);
  glViewport(0, 0, s->screen_w, s->screen_h);
  glLoadIdentity();
  glOrtho(0, s->screen_w, s->screen_h, 0, -1, 1);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

/** Render the game */
void render(Game_State* s) {
  glBegin(GL_QUADS);

  /* Draw background 1 px inset to create a border */
  glColor3f(0.05f, 0.05f, 0.05f);
  glVertex2f(1, 1);
  glVertex2f(s->screen_w - 1, 1);
  glVertex2f(s->screen_w - 1, s->screen_h - 1);
  glVertex2f(1, s->screen_h - 1);

  /* Left bat */
  glColor3f(1, 1, 1);
  glVertex2f(10, s->l_pos);
  glVertex2f(30, s->l_pos);
  glVertex2f(30, s->l_pos + s->l_size);
  glVertex2f(10, s->l_pos + s->l_size);

  /* Right bat */
  glVertex2f(s->screen_w - 10, s->r_pos);
  glVertex2f(s->screen_w - 30, s->r_pos);
  glVertex2f(s->screen_w - 30, s->r_pos + s->r_size);
  glVertex2f(s->screen_w - 10, s->r_pos + s->r_size);

  /* Ball */
  glVertex2f(s->ball_x - 5, s->ball_y - 5);
  glVertex2f(s->ball_x + 5, s->ball_y - 5);
  glVertex2f(s->ball_x + 5, s->ball_y + 5);
  glVertex2f(s->ball_x - 5, s->ball_y + 5);
  glEnd();
}

void do_ball_collisions(Game_State* s) {
  /* Do ball collisions with border */
  if (s->ball_x - 5 < 0) {
    s->ball_x = 5;
    s->ball_velx = - s->ball_velx;
  }
  else if (s->ball_x + 5 > s->screen_w) {
    s->ball_x = s->screen_w - 5;
    s->ball_velx = - s->ball_velx;
  }
  if (s->ball_y - 5 < 0) {
    s->ball_y = 5;
    s->ball_vely = - s->ball_vely;
  }
  else if (s->ball_y + 5 > s->screen_h) {
    s->ball_y = s->screen_h - 5;
    s->ball_vely = - s->ball_vely;
  }

  /* Do with bats */
  if (s->ball_x - 5 < 30) {
    /* left */
    if (s->ball_y + 5 > s->l_pos &&
        s->ball_y - 5 < s->l_pos + s->l_size) {
      s->ball_x = 35;
      s->ball_velx = -s->ball_velx;
    }
  }
  if (s->ball_x + 5 > s->screen_w - 30) {
    /* right */
    if (s->ball_y + 5 > s->r_pos &&
        s->ball_y - 5 < s->r_pos + s->r_size) {
      s->ball_x = s->screen_w - 35;
      s->ball_velx = -s->ball_velx;
    }
  }
}

int main(int argc, char** argv) {
  Game_State* state;
  state = (Game_State*) malloc(sizeof(Game_State));
  init_state(state, 800, 600);
  window = init_glfw(state);
  init_gl(state);

  while(1) {
    if (glfwWindowShouldClose(window)) {
      break;
    }
    /* Apply bat speeds */
    if (w_down) {
      state->l_pos -= 3;
    }
    else if (s_down) {
      state->l_pos += 3;
    }
    if (i_down) {
      state->r_pos -= 3;
    }
    else if (k_down) {
      state->r_pos += 3;
    }

    do_ball_collisions(state);

    /* Apply ball speed */
    state->ball_x += state->ball_velx;
    state->ball_y += state->ball_vely;


    glClear(GL_COLOR_BUFFER_BIT);
    render(state);
    glfwPollEvents();
    glfwSwapBuffers(window);
  }
}

