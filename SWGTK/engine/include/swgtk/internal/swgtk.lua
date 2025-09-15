swgtk = {
  math = {
    pi = 3.14159265358979,
    pi2 = 3.14159265358979 * 2,
    RadiansToDegrees = function (self, radians)
      return (radians / self.pi) * 180.0
    end,
    DegreesToRadians = function (self, degrees)
      return (degrees / 180.0) * self.pi
    end
  }
}
