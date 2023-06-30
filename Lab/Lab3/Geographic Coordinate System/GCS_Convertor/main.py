import math

RANDOM = False

def main():
    print("[Geographic Coordinate System Converter]")
    if RANDOM:
        x, y, z = random_coordinate()
        print("Relative Coordinate")
        print("> x: ", x)
        print("> y: ", y)
        print("> z: ", z)
    else:
        print("Input Relative Coordinate")
        x = float(input("> x: "))
        y = float(input("> y: "))
        z = float(input("> z: "))
    print()

    latitude = 36.372150  # 위도
    longitude = 127.3604000 # 경도
    altitude = 54 # 고도

    earth_radius = 6378137.0 # 지구 반지름
    earth_circumference = 2 * 3.141592 * earth_radius # 지구 둘레
    latitude_per_meter = 360 / earth_circumference
    longitude_per_meter = 360 / (earth_circumference * math.cos(math.radians(latitude))) # 경도 1도당 거리

    relative_coordinate_per_meter = 2.0

    new_latitude = latitude + (y / relative_coordinate_per_meter * latitude_per_meter)
    new_longitude = longitude + (x / relative_coordinate_per_meter * longitude_per_meter)
    new_altitude = altitude + z / relative_coordinate_per_meter

    print("Manipulated Geographic Coordinate System")
    print("> latitude: ", new_latitude)
    print("> longitude: ", new_longitude)
    print("> altitude: ", new_altitude)
    print()

    integer_latitude_longitude_scale = 10 ** 7
    integer_altitude_scale = 10 ** 3

    integer_latitude = int(new_latitude * integer_latitude_longitude_scale)
    integer_longitude = int(new_longitude * integer_latitude_longitude_scale)
    integer_altitude = int(new_altitude * integer_altitude_scale)

    print("Integer Coordinate System")
    print("> latitude: ", integer_latitude)
    print("> longitude: ", integer_longitude)
    print("> altitude: ", integer_altitude)
    print()

def random_coordinate():
    import random
    x = random.randint(-5, 5)
    y = random.randint(-5, 5)
    z = random.randint(0, 10)
    return x, y, z

if __name__ == "__main__":
    main()