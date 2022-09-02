import java.nio.charset.StandardCharsets
import scala.util.Try
import scala.scalanative.unsafe._
import scala.scalanative.libc.{stdlib, string}
import scala.scalanative.loop.EventLoop.loop
import scala.scalanative.loop.LibUV

object Main {
  def main(args: Array[String]): Unit = {
    println("STARTING")

    StdinReader.readLoop { _ =>
      var lastChar: Try[Int] = Try { 0 }
      while (lastChar.isSuccess) {
        lastChar = Try(scala.Console.in.read())
        lastChar.foreach { char =>
          println(
            s"got char `$char`: ${new String(Array(char.toByte), StandardCharsets.UTF_8)}"
          )
        }

        if (lastChar.isFailure) println(s"WHAT $lastChar")
      }
    }
  }
}

object StdinReader {
  import Libuv._

  var callback: Unit => Unit = _

  def readLoop(cb: Unit => Unit): Unit = {
    callback = cb

    val handle =
      stdlib.malloc(uv_handle_size(UV_POLL_T)).asInstanceOf[Ptr[Byte]]

    val status1 = uv_poll_init(loop, handle, 0)
    require(status1 == 0, s"failed to init poll handle: $status1")

    val status2 = uv_poll_start(handle, UV_READABLE, pollReadCB)
    require(status2 == 0, s"failed to start polling: $status2")
  }

  private val pollReadCB: CFuncPtr3[Ptr[Byte], Int, Int, Unit] = (
      handle: Ptr[Byte],
      status: Int,
      events: Int
  ) => {
    callback(())
  }

  private val UV_READABLE = 1
  private val UV_POLL_T = 8
}

@link("uv")
@extern
object Libuv {
  def uv_handle_size(handle_type: Int): CSize = extern
  def uv_poll_init(
      loop: LibUV.Loop,
      handle: Ptr[Byte],
      fd: Int
  ): Int = extern
  def uv_poll_start(
      handle: Ptr[Byte],
      events: Int,
      cb: CFuncPtr3[Ptr[Byte], Int, Int, Unit]
  ): Int = extern
}
