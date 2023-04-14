# Author: Chip Nguyen
# Date: 10/08/2022
# Purpose: Lab 1 - Pong

from cs1lib import *

# Define constant variables
WINDOW_H = 400
WINDOW_W = 400
PADDLE_W = 20
PADDLE_H = 80
PADDLE_MOVEMENT = 10
BALL_RADIUS = 12

# Define main keys used
KEY_LEFT_UP = "a"
KEY_LEFT_DOWN = "z"
KEY_RIGHT_UP = "k"
KEY_RIGHT_DOWN = "m"
KEY_NEW_GAME = " "
KEY_QUIT = "q"

# State variables for paddles
left_up = False
left_down = False
right_up = False
right_down = False

# State variable for if game is running
is_game_running = False

# Position of paddle 1
px1 = 0
py1 = 0

# Position of paddle 2
px2 = WINDOW_W - PADDLE_W
py2 = WINDOW_H - PADDLE_H

# Location and velocity of the ball
bx = 200
by = 200
vx = 5
vy = 5

def set_background(): # Draws background
    # Set background color
    set_clear_color(0.53,0.81,0.94) # Baby blue
    clear()

    # Middle line
    enable_stroke()
    set_stroke_width(3)
    set_stroke_color(1,1,1)
    draw_line(200,0,200,400)

    # Circle in the middle
    draw_circle(200,200,50)
    set_fill_color(0.53,0.81,0.94) # Darker baby blue
    draw_circle(200,200,48)

# Function to draw the paddles
def draw_paddle(x,y):
    set_fill_color(1,0.6,0.7)
    draw_rectangle(x,y,PADDLE_W,PADDLE_H)

# Function that draws the ball and updates its position
def draw_ball():
    global bx,by,vx,vy

    set_fill_color(0.94,0.79,0.41) # Baby yellow
    draw_circle(bx,by,BALL_RADIUS)

    # Updates the location of the ball
    bx = bx + vx
    by = by + vy

# Checks if the ball hits the left paddle. If yes, return True
# 1st condition checks if the ball is coming towards the left paddle
# 2nd condition makes sure the y-coordinate of the ball is within the y-values of the paddle's bottom and top
# 3rd condition makes sure the edge of the ball touches the paddle
def hit_left_paddle(bx,by,top_paddle,bottom_paddle):
    if vx < 0 and top_paddle <= by <= bottom_paddle and bx - BALL_RADIUS <= PADDLE_W:
        return True

# Checks if the ball hits the right paddle. If yes, return True. Explanations of conditions are the same as above
def hit_right_paddle(bx,by,top_paddle,bottom_paddle):
    if vx > 0 and top_paddle <= by <= bottom_paddle and bx + BALL_RADIUS >= (WINDOW_W - PADDLE_W):
        return True

# Check if the ball hits a vertical wall. If yes, return True
def hit_vertical_wall(x): # x will take the value of the x-coordinate of the ball plus/minus its radius at the time of contact with the wall
    if x >= WINDOW_W or x <= 0:
        return True

# Check if the ball hits a horizontal wall. If yes, return True
def hit_horizontal_wall(y): # y will take the value of the y-coordinate of the ball plus/minus its radius at the time of contact with the wall
    if y >= WINDOW_H or y <= 0:
        return True

# Set up key interactions
def kpress(key):
    global is_game_running,left_up,left_down,right_up,right_down

    # Start a new game when the space bar is pressed
    if key == KEY_NEW_GAME:
        is_game_running = True

    # Move left paddle when relevant keys are pressed
    if key == KEY_LEFT_UP:
        left_up = True
    if key == KEY_LEFT_DOWN:
        left_down = True

    # Move right paddle when relevant keys are pressed
    if key == KEY_RIGHT_UP:
        right_up = True
    if key == KEY_RIGHT_DOWN:
        right_down = True

    # Quit when "q" is pressed
    if key == KEY_QUIT:
        cs1_quit()

def krelease(key):
    global left_down,left_up,right_up,right_down

    # Stops left paddle
    if key == KEY_LEFT_UP:
        left_up = False
    if key == KEY_LEFT_DOWN:
        left_down = False

    # Stops right paddle
    if key == KEY_RIGHT_UP:
        right_up = False
    if key == KEY_RIGHT_DOWN:
        right_down = False

# Helper function that implements so that the game only starts when space bar is pressed;
# in other words, when is_game_running is false
def start():
    global bx, by

    set_background()
    disable_stroke()
    draw_paddle(px1,py1)
    draw_paddle(px2,py2)

    if is_game_running == False:  # Sets the initial screen with instruction text and no ball
        enable_stroke()
        set_stroke_color(0, 0, 0)
        set_font_size(15)
        draw_text("Press Space Bar to Start",120,200)
    else: # When space bar is pressed, instruction text disappears, ball appears and starts moving immediately
        set_stroke_color(1,1,1)
        disable_stroke() 
        draw_ball()

# Create the game
def pong():
    global px1,py1,px2,py2,by,bx,vx,vy,left_up,left_down,right_up,right_down,is_game_running

    start()

    # Left paddle movement
    if left_up and py1 > 0:
        py1 = py1 - PADDLE_MOVEMENT
    if left_down and py1 < WINDOW_H - PADDLE_H:
        py1 = py1 + PADDLE_MOVEMENT

    # Right paddle movement
    if right_up and py2 > 0:
        py2 = py2 - PADDLE_MOVEMENT
    if right_down and py2 < WINDOW_H - PADDLE_H:
        py2 = py2 + PADDLE_MOVEMENT

    # Reverse the direction of the ball when it hits a paddle
    if hit_left_paddle(bx,by,py1,py1 + PADDLE_H) or hit_right_paddle(bx,by,py2,py2 + PADDLE_H):
        vx = - vx

    # Reverse the direction of the ball when it hits one of the horizontal walls
    if hit_horizontal_wall(by - BALL_RADIUS) or hit_horizontal_wall(by + BALL_RADIUS):
        vy = - vy

    # When the ball hits a vertical wall, the game resets and only continues when the space bar is pressed.
    # The ball position is set back to the middle of the screen
    if hit_vertical_wall(bx + BALL_RADIUS) or hit_vertical_wall(bx - BALL_RADIUS):
        is_game_running = False
        bx = 200
        by = 200

start_graphics(pong,key_press=kpress,key_release=krelease)
