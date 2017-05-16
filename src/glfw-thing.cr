require "lib_gl"
require "glfw"

# hints = {
#   LibGLFW3::OPENGL_PROFILE        => LibGLFW3::OPENGL_CORE_PROFILE,
#   LibGLFW3::OPENGL_FORWARD_COMPAT => 1,
#   LibGLFW3::CONTEXT_VERSION_MAJOR => 3,
#   LibGLFW3::CONTEXT_VERSION_MINOR => 3,
# }

window = GLFW::Window.new(800, 600, "Hello!", {
  LibGLFW3::SAMPLES   => 2,
  LibGLFW3::DECORATED => 1,
})

window.make_current

GLFW.swap_interval = 1

LibGLFW3.setWindowRefreshCallback(window, ->(window) {
  LibGLFW3.getFramebufferSize(window, out width, out height)
  fb_size = {width: width, height: height}
  draw(fb_size)
  LibGLFW3.swapBuffers(window)
})

def draw(fb_size)
  ratio = fb_size[:width].to_f / fb_size[:height].to_f

  LibGL.viewport(0, 0, 500, 500)
  LibGL.clear(LibGL::COLOR_BUFFER_BIT)
  LibGL.matrixMode(LibGL::PROJECTION)
  LibGL.loadIdentity
  LibGL.ortho(1, 1, -1.0, 1.0, 1.0, -1.0)
  LibGL.matrixMode(LibGL::MODELVIEW)

  LibGL.pushMatrix

  LibGL.loadIdentity
  LibGL.rotatef(GLFW.time * 50.0, 0.0, 0.0, 1.0)

  LibGL.color3f(1.0, 0.0, 0.0)
  LibGL.rectd(-0.7, -0.7, -0.3, -0.3)

  LibGL.popMatrix

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

  LibGL.viewport(0, 0, 100, 100)
  LibGL.color3f(0.0, 0.0, 1.0)
  LibGL.rectd(-1, -1, 1, 1)
end

drawing = Channel(Int8).new

spawn do
  while drawing.receive
    draw(window.framebuffer_size)
    window.swap_buffers
  end
end

until window.should_close?
  drawing.send(1.to_i8)
  GLFW.poll_for_events
end
