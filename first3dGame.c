#include "raylib.h"
#include "raymath.h"
#include "rcamera.h"

// make box method
BoundingBox makeBox(Vector3 position, Vector3 size)
{
   Vector3 halfSize = Vector3Scale(size, 0.5f);
BoundingBox box = {
    Vector3Subtract(position, halfSize), 
    Vector3Add(position, halfSize)
    };
    return box;
}

// platform function
bool platformCollisionDetector(Vector3 *playerPos, Vector3 playerSize, Vector3 platformPos, Vector3 platformSize, Vector3 *velocity){
    
    // bounding boxes
    BoundingBox playerBox = makeBox(*playerPos, playerSize);
    BoundingBox platformBox = makeBox(platformPos, platformSize);
    
    // grounded
    bool grounded = false;
    
    
    if (CheckCollisionBoxes(playerBox, platformBox) 
    && playerBox.min.y >= platformBox.max.y - 0.01f
    ){
        
        grounded = true;
        velocity->y = 0.0f;
        playerPos->y = platformBox.max.y + (playerSize.y/2);
        
    } else if(playerPos->y - (playerSize.y/2) < platformPos.y + (platformSize.y/2) // under the top
        && playerPos->x + (playerSize.x/2) > platformPos.x - (platformSize.x/2)    // stays on the right x for the Left Side
        && playerPos->x + (playerSize.x/2) < platformPos.x + (platformSize.x/2)      
        && playerPos->z + (playerSize.z/2) > platformPos.z - (platformSize.z/2)    // stays on the right Z
        && playerPos->z - (playerSize.z/2) < platformPos.z + (platformSize.z/2)
        ){
        
        grounded = true;
        playerPos->x = platformBox.min.x - (playerSize.x / 2);
        
    } else if (playerPos->y - (playerSize.y/2) < platformPos.y + (platformSize.y/2) // under the top
        && playerPos->x - (playerSize.x/2) < platformPos.x + (platformSize.x/2) // stays on the right x for the right side
        && playerPos->x - (playerSize.x/2) > platformPos.x - (platformSize.x/2) 
        && playerPos->z + (playerSize.z/2) > platformPos.z - (platformSize.z/2) // stays on the right Z
        && playerPos->z - (playerSize.z/2) < platformPos.z + (platformSize.z/2)
        ){
            
        grounded = true;
        playerPos->x = platformBox.max.x + (playerSize.x / 2);
            
    }else if (playerPos->y - (playerSize.y/2) < platformPos.y + (platformSize.y/2) // under the top
        && playerBox.max.z >= platformBox.min.z + 0.1f // stays on the right Z for the left side
        && playerBox.max.z < platformBox.max.z + 0.1f
        && playerBox.min.x >= platformBox.min.x - 0.1f
        && playerBox.max.x <= platformBox.max.x - 0.1f
    ){
        
        grounded = true;
        velocity->y = 0.0f;
        playerPos->z = platformBox.min.z - (playerSize.z/2);
        
    } else if (playerPos->y - (playerSize.y/2) < platformPos.y + (platformSize.y/2) // under the top 
        && playerBox.min.z >= platformBox.max.z - 0.1f // stays on the right Z for the right side
        && playerBox.min.z < platformBox.min.z - 0.1f
        && playerBox.min.x >= platformBox.min.x - 0.1f
        && playerBox.max.x <= platformBox.max.x - 0.1f
    ){
        
        grounded = true;
        velocity->y = 0.0f;
        playerPos->z = platformBox.max.z + (playerSize.z/2);
        
    }
    
    return grounded;
}









//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [models] example - geometric shapes");
    Vector3 playerPos = {-5.0f, 0.0f, 0.0f};
    // Define the camera to look into our 3d world
    Camera camera = { 0 };
    camera.position = (Vector3){ playerPos.x - 5.0f, playerPos.y + 3.0f, playerPos.z - 5.0f }; // Behind and above
    camera.target = playerPos; // Look at the cube
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;            // Camera projection type

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    
    // player variables
    Vector3 playerSize = {2.0f, 2.0f, 2.0f};
    BoundingBox playerBox = makeBox(playerPos, playerSize);
    float playerSpeed = 5.0f;
    const char* movementState = "Idle";
    // platform variables
    Vector3 platform1 = {0.0f, 0.5f, 0.0f};
    Vector3 platform1Size = {3.0f, 1.0f, 2.0f};
     // direction
     Vector3 direction = { 0.0f, 0.0f, 0.0f };
     //velocity
     Vector3 velocity = {0.0f, 0.0f, 0.0f};
     float gravity = -9.8f;
     float groundLevel = 0;
    // Main game loop
    
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {   
        //------------------------------------------------------------------------------------
        // move camera
        camera.position = Vector3Add(playerPos, (Vector3){ -5.0f, 5.0f, 0.0f });
        camera.target = playerPos;                                      
        // normal key movement
        direction = (Vector3){ 0.0f, 0.0f, 0.0f };
        // current movement state
        movementState = "Idle";
        // movement
        if (IsKeyDown(KEY_S)) direction.x -= 1.0f; // forward
        if (IsKeyDown(KEY_W)) direction.x += 1.0f; // backward
        if (IsKeyDown(KEY_A)) direction.z -= 1.0f; // left
        if (IsKeyDown(KEY_D)) direction.z += 1.0f; // right
            // sprint
            if (IsKeyDown(KEY_LEFT_SHIFT)) {
                playerSpeed = 15.0f;
           
            }
            // crouch
            if (IsKeyDown(KEY_C) && !IsKeyDown(KEY_LEFT_SHIFT)) {
                playerSpeed = 1.0f;
                
            }
            // gravity REMEMBER
            velocity.y += gravity * GetFrameTime();
            playerPos.y += velocity.y * GetFrameTime();
            //normal speed
            if (!IsKeyDown(KEY_C) && !IsKeyDown(KEY_LEFT_SHIFT)){
                playerSpeed = 5.0f;
            // movement states  
            }
            if (playerSpeed == 1.0f){
                movementState = "crouching";
            } else if (playerSpeed == 15.0f){
                movementState = "sprinting";
            } else if (playerSpeed == 5.0f){
                movementState = "walking";
            }
            if (direction.x == 0.0f && direction.y == groundLevel && direction.z == 0.0f){
                movementState = "idle";
            }
        // rememeber this is how you normalize a vector
        if (Vector3Length(direction) > 0.0f) {
            direction = Vector3Normalize(direction); // Normalize direction
            playerPos.x += direction.x * playerSpeed * GetFrameTime();
            playerPos.z += direction.z * playerSpeed * GetFrameTime();
            
        }    
        // playerBox hitbox
        playerBox = makeBox(playerPos, playerSize);
        
        bool isGrounded = platformCollisionDetector(&playerPos, playerSize, platform1, platform1Size, &velocity);
            if (playerPos.y <= groundLevel){
            playerPos.y = groundLevel;
            velocity.y = 0.0f;
            isGrounded = true;
            }
            // jumping
            if (IsKeyDown(KEY_SPACE) && isGrounded == true){
                velocity.y = 6.5f;
            }
            if (IsKeyDown(KEY_SPACE) && isGrounded == true && IsKeyDown(KEY_LEFT_SHIFT)){
                velocity.y = 8.5f;
            }        
        // clamp jumping
            if (isGrounded == false){
                movementState = "in air";
            }
            
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        // backroungcolor
        ClearBackground(WHITE);
        // 3d mode
            BeginMode3D(camera);
            // player cube
            DrawCubeWires(playerPos, 2.0f, 2.0f, 2.0f, BLACK);
            DrawCube(playerPos, 2.0f, 2.0f, 2.0f, RED);
            // draw shadow
            if (isGrounded == false){
                DrawCube((Vector3){playerPos.x, playerPos.y - 2.0f, playerPos.z}, 2.0f, 0.01f, 2.0f, LIGHTGRAY);
            }
            // obstacle cube
            DrawCubeWires(platform1, 3.0f, 1.0f, 2.0f, BLACK); // platform1
            DrawCube(platform1, 3.0f, 1.0f, 2.0f, WHITE);
            // end 3d
            EndMode3D();
            // movement state text
            DrawText(TextFormat("State: %s", movementState), 325, 15, 20, BLACK);
            // fps test
            DrawFPS(10, 10);
        // end drawing
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}