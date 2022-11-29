// Header file for Tyler Brown

extern void set_difficulty(int a);
extern int check_autoplay();
extern void show_autoplay(int a, int xres, int yres);
extern void show_difficulty(int a, int xres, int yres);
extern void set_autoplay(int a, float &paddlepos0, float &paddlepos1);
extern void show_winner(int a, int xres, int yres);
extern void show_intro_screen(GLuint introTexture, int xres, int yres);
extern void show_background(GLuint backgroundTexture, int xres, int yres);
extern void reset_brick_pos(int xres);
class Bricks;
extern void make_bricks(int x, int y);
extern void check_brick_hit(float &puckw, float &puckpos1, float &puckpos2, float &puckvel0, float &puckvel1);
extern void draw_bricks();
extern void move_bricks(int xres, int toggle);
extern void set_goals(int x, int y);
extern int check_player_goal(float puckpos0, float puckpos1, float puckw);
extern int check_ai_goal(float puckpos0, float puckpos1, float puckw);
extern void ai_paddle_physics(float puckpos0, float puckpos1, float puckw, float &puckvel1, float &puckvel0, int y);
extern void check_score(int &player_score, int &ai_score, int &game_over, int &game_over_timer, int &winner);
