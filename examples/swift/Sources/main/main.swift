import WebrogueGFX
import WebrogueGFXRaw

typealias Vertex = (Float, Float, Float, Float, Float)

let vertex_shader_text =
    """
    #version 100
    precision mediump float;
    uniform float time;
    attribute vec3 vCol;
    attribute vec2 vPos;
    varying vec3 color;
    void main()
    {
        float s = sin(time);
        float c = cos(time);
        mat2 m = mat2(c, s, -s, c);
        gl_Position = vec4(vPos * m, 0.0, 1.0);
        color = vCol;
    }
    """

let fragment_shader_text =
    """
    #version 100
    precision mediump float;
    varying vec3 color;
    void main()
    {
        gl_FragColor = vec4(color, 1.0);
    }
    """
let vertices: [Vertex] = [
    (-0.6, -0.4, 1.0, 0.0, 0.0),
    (0.6, -0.4, 0.0, 1.0, 0.0),
    (0.0, 0.6, 0.0, 0.0, 1.0),
]

func main() {
    print("Hello triangle")

    let window = Window()
    window.initGL()
    let glGenBuffers: PFNGLGENBUFFERSPROC = window.loadGLPtr("glGenBuffers")
    let glBindBuffer: PFNGLBINDBUFFERPROC = window.loadGLPtr("glBindBuffer")
    let glBufferData: PFNGLBUFFERDATAPROC = window.loadGLPtr("glBufferData")
    let glCreateShader: PFNGLCREATESHADERPROC = window.loadGLPtr("glCreateShader")
    let glShaderSource: PFNGLSHADERSOURCEPROC = window.loadGLPtr("glShaderSource")
    let glCompileShader: PFNGLCOMPILESHADERPROC = window.loadGLPtr("glCompileShader")
    let glCreateProgram: PFNGLCREATEPROGRAMPROC = window.loadGLPtr("glCreateProgram")
    let glAttachShader: PFNGLATTACHSHADERPROC = window.loadGLPtr("glAttachShader")
    let glLinkProgram: PFNGLLINKPROGRAMPROC = window.loadGLPtr("glLinkProgram")
    let glGetAttribLocation: PFNGLGETATTRIBLOCATIONPROC = window.loadGLPtr("glGetAttribLocation")
    let glEnableVertexAttribArray: PFNGLENABLEVERTEXATTRIBARRAYPROC = window.loadGLPtr(
        "glEnableVertexAttribArray")
    let glVertexAttribPointer: PFNGLVERTEXATTRIBPOINTERPROC = window.loadGLPtr(
        "glVertexAttribPointer")
    let glUseProgram: PFNGLUSEPROGRAMPROC = window.loadGLPtr("glUseProgram")
    let glDrawArrays: PFNGLDRAWARRAYSPROC = window.loadGLPtr("glDrawArrays")
    // let glGetError: PFNGLGETERRORPROC = window.loadGLPtr("glGetError")
    let glFlush: PFNGLFLUSHPROC = window.loadGLPtr("glFlush")
    let glGetUniformLocation: PFNGLGETUNIFORMLOCATIONPROC = window.loadGLPtr("glGetUniformLocation")
    let glUniform1f: PFNGLUNIFORM1FPROC = window.loadGLPtr("glUniform1f")
    let glClear: PFNGLCLEARPROC = window.loadGLPtr("glClear")

    var vertex_buffer: GLuint = 0
    glGenBuffers(1, &vertex_buffer)
    glBindBuffer(GLenum(GL_ARRAY_BUFFER), vertex_buffer)
    glBufferData(
        GLenum(GL_ARRAY_BUFFER), vertices.count * MemoryLayout<Vertex>.size, vertices,
        GLenum(GL_STATIC_DRAW)
    )

    let vertex_shader = glCreateShader(GLenum(GL_VERTEX_SHADER))
    vertex_shader_text.withCString { vertex_shader_ptr in
        let shaders: [UnsafePointer<Int8>?] = [vertex_shader_ptr]
        shaders.withUnsafeBufferPointer { shaders_ptr in
            glShaderSource(vertex_shader, 1, shaders_ptr.baseAddress, nil)
        }
    }
    glCompileShader(vertex_shader)

    let fragment_shader = glCreateShader(GLenum(GL_FRAGMENT_SHADER))
    fragment_shader_text.withCString { fragment_shader_text_ptr in
        let shaders: [UnsafePointer<Int8>?] = [fragment_shader_text_ptr]
        shaders.withUnsafeBufferPointer { shaders_ptr in
            glShaderSource(fragment_shader, 1, shaders_ptr.baseAddress, nil)
        }
    }
    glCompileShader(fragment_shader)

    let program = glCreateProgram()
    glAttachShader(program, vertex_shader)
    glAttachShader(program, fragment_shader)
    glLinkProgram(program)

    let time_location = GLint(glGetUniformLocation(program, "time"))
    let vpos_location = GLuint(glGetAttribLocation(program, "vPos"))
    let vcol_location = GLuint(glGetAttribLocation(program, "vCol"))

    glEnableVertexAttribArray(vpos_location)
    glEnableVertexAttribArray(vcol_location)

    glVertexAttribPointer(
        vpos_location, 2, GLenum(GL_FLOAT), GLboolean(GL_FALSE), GLsizei(MemoryLayout<Vertex>.size),
        UnsafeRawPointer.init(
            bitPattern: MemoryLayout<Vertex>.offset(of: \.0)!
        )
    )
    glVertexAttribPointer(
        vcol_location, 3, GLenum(GL_FLOAT), GLboolean(GL_FALSE), GLsizei(MemoryLayout<Vertex>.size),
        UnsafeRawPointer.init(
            bitPattern: MemoryLayout<Vertex>.offset(of: \.2)!
        )
    )

    var iteration = 0
    while true {
        iteration += 1
        while let event = window.pullEvent() {
            switch event {
            case .quit:
                return
            case .mouseDown(let eventData):
                print("A mouse button pressed at \(eventData.x) \(eventData.y)")
            default:
                break
            }
        }

        // to lazy to clear

        glUseProgram(program)
        glUniform1f(time_location, GLfloat(iteration) / 100)
        glClear(GLbitfield(GL_COLOR_BUFFER_BIT))
        glDrawArrays(GLenum(GL_TRIANGLES), 0, 3)
        glFlush()
        window.present()
    }
}

main()
