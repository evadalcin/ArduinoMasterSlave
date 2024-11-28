using ArduinoControlApp.Services;
using Microsoft.AspNetCore.Mvc;
using System.Threading.Tasks;

namespace ArduinoControlApp.Controllers
{
    public class HomeController : Controller
    {
        private readonly ArduinoReader _arduinoReader;

        public HomeController(ArduinoReader arduinoReader)
        {
            _arduinoReader = arduinoReader;
        }

        public IActionResult Index()
        {
            return View();
        }

        [HttpPost]
        public IActionResult MotorOn()
        {
            _arduinoReader.MotorOnCommand();
            ViewBag.Message = "Motore acceso!";
            return View("Index");
        }

        [HttpPost]
        public IActionResult MotorOff()
        {
            _arduinoReader.MotorOffCommand();
            ViewBag.Message = "Motore spento!";
            return View("Index");
        }

        [HttpPost]
        public IActionResult AlarmOn()
        {
            _arduinoReader.AlarmOnCommand();
            ViewBag.Message = "Allarme attivato!";
            return View("Index");
        }

        [HttpPost]
        public IActionResult AlarmOff()
        {
            _arduinoReader.AlarmOffCommand();
            ViewBag.Message = "Allarme disattivato!";
            return View("Index");
        }
    }
}
