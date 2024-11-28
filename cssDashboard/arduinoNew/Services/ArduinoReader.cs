using System;
using System.IO.Ports;

namespace ArduinoControlApp.Services
{
    public class ArduinoReader
    {
        private SerialPort _serialPort;

        public event Action MotorOn;
        public event Action MotorOff;
        public event Action AlarmOn;
        public event Action AlarmOff;

        public ArduinoReader(string portName, int baudRate = 9600)
        {
            _serialPort = new SerialPort(portName, baudRate);
            _serialPort.DataReceived += SerialPort_DataReceived;

            if (!_serialPort.IsOpen)
            {
                _serialPort.Open();  
            }
        }

        public void Start()
        {
            try
            {
                if (!_serialPort.IsOpen)
                {
                    _serialPort.Open();  
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine("Errore durante l'apertura della porta seriale: " + ex.Message);
            }
        }
        public void Stop()
        {
            if (_serialPort.IsOpen)
            {
                _serialPort.Close(); 
            }
        }

        public void MotorOnCommand()
        {
            if (!_serialPort.IsOpen)
            {
                _serialPort.Open();  
            }

            _serialPort.WriteLine("motoron");  
            MotorOn?.Invoke(); 
        }
        public void MotorOffCommand()
        {
            if (!_serialPort.IsOpen)
            {
                _serialPort.Open();  
            }

            _serialPort.WriteLine("motoroff");  
            MotorOff?.Invoke();  
        }

        public void AlarmOnCommand()
        {
            if (!_serialPort.IsOpen)
            {
                _serialPort.Open(); 
            }

            _serialPort.WriteLine("alarmon");  
            AlarmOn?.Invoke();  
        }

        public void AlarmOffCommand()
        {
            if (!_serialPort.IsOpen)
            {
                _serialPort.Open();  
            }

            _serialPort.WriteLine("alarmoff");  
            AlarmOff?.Invoke();  
        }

        private void SerialPort_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            try
            {
                string data = _serialPort.ReadLine();
                Console.WriteLine("Dati ricevuti: " + data);
            }
            catch (TimeoutException)
            {
            }
        }
    }
}
