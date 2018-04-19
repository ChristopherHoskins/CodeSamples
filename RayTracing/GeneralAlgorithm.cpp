// One pass will miss a large portion of pixels within our scene and thus
// leaving all sorts of black pixels and thus requiing multiple passes 
// consolidating all found color contributions for each pixel
for (int pass = 1; pass < number_of_passes; ++pass)
{
  for (int y = 0; y < height; y++)
  {
    float ry = ray_time->camera_.ry;
    float rx = ry * (float(width) / height);
    
    // Gather rotation information from the current scenes camera
    Quaternionf Q = ray_time->camera_.orientation;
    Vector3f X = rx * Q._transformVector(Vector3f::UnitX());
    Vector3f Y = ry * Q._transformVector(Vector3f::UnitY());
    Vector3f Z = -1 * Q._transformVector(Vector3f::UnitZ());

    for (int x = 0; x < width; x++)
    {
      // Create viewing plane with cameras orientation
      Vector2f screen_space;
      // Myrandom(RNGen) is cheap anti-aliasing (dont worry about that for now)
      screen_space.x() = ((2 * (x + .5f)) / width) - 1;
      screen_space.y() = ((2 * (y + .5f)) / height) - 1;
      Vector3f direction = screen_space.x() * X + screen_space.y() * Y + Z;

      // Start ray tracing includes shape intersections and generating new rays
      Ray ray(ray_time->camera_.eye, direction.normalized());
      // TracePath will return the total color contribution found on the pixel
      // selected which is then put into the final produced image.
      image[y*width + x] += TracePath(&ray);
    }
  }
}
