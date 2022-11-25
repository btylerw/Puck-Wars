// Header file for Tyler Brown

extern void set_difficulty(int a);
extern int check_autoplay();
extern void set_autoplay(int a);
extern void show_intro_screen(GLuint introTexture, int xres, int yres);
extern void show_background(GLuint backgroundTexture, int xres, int yres);
extern void reset_brick_pos(int xres);
class Bricks;
extern void make_bricks(int x, int y);
extern void check_brick_hit(float &puckw, float &puckpos1, float &puckpos2, float &puckvel0, float &puckvel1);
extern void draw_bricks();
extern void move_bricks(int xres);
extern void set_goals(int x, int y);
extern int check_player_goal(float puckpos0, float puckpos1, float puckw);
extern int check_ai_goal(float puckpos0, float puckpos1, float puckw);
extern void ai_paddle_physics(float puckpos0, float puckpos1, float puckw, float &puckvel1, float &puckvel0, int y);
