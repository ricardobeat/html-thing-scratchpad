require "lib_gl"
require "lib_glfw3"

draw_stuff = ->(window : LibGLFW3::Window) {
  LibGLFW3.getFramebufferSize(window, out width, out height)
  ratio = width/height

  LibGL.viewport(0, 0, width, height)
  LibGL.clear(LibGL::COLOR_BUFFER_BIT)
  LibGL.matrixMode(LibGL::PROJECTION)
  LibGL.loadIdentity
  LibGL.ortho(-ratio, ratio, -1.0, 1.0, 1.0, -1.0)
  LibGL.matrixMode(LibGL::MODELVIEW)

  LibGL.loadIdentity
  LibGL.rotatef(LibGLFW3.getTime * 50.0, 0.0, 0.0, 1.0)

  LibGL.color3f(1.0, 0.0, 0.0)
  LibGL.rectd(-0.7, -0.7, -0.3, -0.3)

  LibGL.begin(LibGL::POLYGON)
  LibGL.color3f(1.0, 0.0, 0.0)
  LibGL.vertex2f(0.0, 0.0)
  LibGL.color3f(0.0, 1.0, 0.0)
  LibGL.vertex2f(0.3, 0.0)
  LibGL.color3f(0.0, 0.0, 1.0)
  LibGL.vertex2f(0.3, 0.3)
  LibGL.color3f(1.0, 1.0, 1.0)
  LibGL.vertex2f(0.0, 0.3)
  LibGL.end

  LibGLFW3.swapBuffers(window)
}

if LibGLFW3.init != 0
  error_callback = ->(error : Int32, description : Int8*) {
    puts "Error #{error}: #{String.new(description.as(UInt8*))}"
  }

  LibGLFW3.setErrorCallback(error_callback)

  LibGLFW3.windowHint(LibGLFW3::OPENGL_PROFILE, LibGLFW3::OPENGL_CORE_PROFILE)
  LibGLFW3.windowHint(LibGLFW3::OPENGL_FORWARD_COMPAT, 1)
  LibGLFW3.windowHint(LibGLFW3::CONTEXT_VERSION_MAJOR, 3)
  LibGLFW3.windowHint(LibGLFW3::CONTEXT_VERSION_MINOR, 3)

  window = LibGLFW3.createWindow(800, 600, "Hello Crystal!", nil, nil)

  if window
    LibGLFW3.makeContextCurrent(window)
    LibGLFW3.swapInterval(1)

    # LibGLFW3.setWindowRefreshCallback(window, draw_stuff)

    while LibGLFW3.windowShouldClose(window) == 0
      LibGLFW3.getFramebufferSize(window, out width, out height)
      ratio = width.to_f / height.to_f
      LibGL.viewport(0, 0, width, height)
      LibGL.clear(LibGL::COLOR_BUFFER_BIT)
      LibGL.matrixMode(LibGL::PROJECTION)
      LibGL.loadIdentity
      LibGL.ortho(-ratio, ratio, -1.0, 1.0, 1.0, -1.0)
      LibGL.matrixMode(LibGL::MODELVIEW)

      LibGL.begin(LibGL::POLYGON)
      LibGL.color3f(1.0, 0.0, 0.0)
      LibGL.vertex2f(0.0, 0.0)
      LibGL.color3f(0.0, 1.0, 0.0)
      LibGL.vertex2f(0.3, 0.0)
      LibGL.color3f(0.0, 0.0, 1.0)
      LibGL.vertex2f(0.3, 0.3)
      LibGL.color3f(1.0, 1.0, 1.0)
      LibGL.vertex2f(0.0, 0.3)
      LibGL.end

      LibGLFW3.swapBuffers(window)
      LibGLFW3.pollEvents
    end

    LibGLFW3.destroyWindow(window)
  else
    puts "Failed to create window"
  end

  LibGLFW3.terminate
else
  puts "GLFW failed to initialize"
end
