using ArduinoControlApp.Services;
using Microsoft.AspNetCore.Mvc;


namespace ArduinoControlApp.Controllers
{
    public class ArduinoController : Controller
    {
        private readonly ArduinoReader _arduinoReader;

        public ArduinoController(ArduinoReader arduinoReader)
        {
            _arduinoReader = arduinoReader;

            _arduinoReader.MotorOn += () => ViewBag.Message = "Motor is ON";
            _arduinoReader.MotorOff += () => ViewBag.Message = "Motor is OFF";
            _arduinoReader.AlarmOn += () => ViewBag.Message = "Alarm is ON";
            _arduinoReader.AlarmOff += () => ViewBag.Message = "Alarm is OFF";
        }

        public IActionResult Index()
        {
            return View();
        }

        [HttpPost]
        public IActionResult MotorOn()
        {
            _arduinoReader.MotorOnCommand();  
            return View("Index"); 
        }

        [HttpPost]
        public IActionResult MotorOff()
        {
            _arduinoReader.MotorOffCommand();  
            return View("Index");  
        }

        [HttpPost]
        public IActionResult AlarmOn()
        {
            _arduinoReader.AlarmOnCommand();  
            return View("Index"); 
        }

        [HttpPost]
        public IActionResult AlarmOff()
        {
            _arduinoReader.AlarmOffCommand();  
            return View("Index"); 
        }
    }
}
