extern unsigned int manage_state(unsigned int s);
extern void help_screen(GLuint help_screen_img, int xres, int yres);
extern void draw_circle(float cx, float cy, float radius, int segs);
extern void drawPowerUps(float* p1, int sz);
extern float speedUp(float* vel);
extern void reset(float* puckPos, int xres, int yres, float* velocity, bool firstTime, int brick_feature, int powerUp, int increaseSZ);
extern void limit_speed(float* vel, int speedCap);