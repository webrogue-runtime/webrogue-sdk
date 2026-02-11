import WebrogueGFXRaw

public enum Event {
    case quit
    case mouseUp(eventData: webrogue_event_mouse_up)
    case mouseDown(eventData: webrogue_event_mouse_down)
    case mouseMotion(eventData: webrogue_event_mouse_motion)
}

public class Window {
    public init() {
        webroguegfx_make_window()
    }

    public func initGL() {
        webroguegfx_init_gl()
    }

    public func loadGLPtr<T>(_ proc: String) -> T {
        let source_ptr = UnsafeMutablePointer<T>.allocate(capacity: 1)

        source_ptr.withMemoryRebound(to: UnsafeMutableRawPointer.self, capacity: 1) { pointer in
            pointer.pointee = proc.withCString {
                webroguegfx_gl_loader($0)
            }!
        }
        return source_ptr.pointee
    }

    public func present() {
        webroguegfx_present()
    }

    public func pullEvent() -> Event? {
        let event = webroguegfx_poll()
        return switch event.type {
        case WEBROGUE_EVENT_TYPE_QUIT:
            .quit
        case WEBROGUE_EVENT_TYPE_MOUSE_UP:
            .mouseUp(eventData: event.inner.mouse_up)
        case WEBROGUE_EVENT_TYPE_MOUSE_DOWN:
            .mouseDown(eventData: event.inner.mouse_down)
        case WEBROGUE_EVENT_TYPE_MOUSE_MOTION:
            .mouseMotion(eventData: event.inner.mouse_motion)
        case WEBROGUE_EVENT_TYPE_INVALID:
            nil
        default:
            pullEvent()
        }
    }
}
