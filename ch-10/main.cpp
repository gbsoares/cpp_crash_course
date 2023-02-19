#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <functional>
#include <stdexcept>

struct SpeedUpdate
{
    double velocity_mps;
};

struct CarDetected
{
    double distance_m;
    double velocity_mps;
};

struct SpeedLimitDetected
{
    unsigned short speed_mps;
};

struct BreakCommand
{
    double time_to_collision_s;
};

typedef std::function<void(const SpeedUpdate&)> SpeedUpdateCallback;
typedef std::function<void(const CarDetected&)> CarDetectedCallback;
typedef std::function<void(const SpeedLimitDetected&)> SpeedLimitCallback;

struct IServiceBus
{
    virtual ~IServiceBus() = default;
    virtual void publish(const BreakCommand&) = 0;
    virtual void subscribe(SpeedUpdateCallback) = 0;
    virtual void subscribe(CarDetectedCallback) = 0;
    virtual void subscribe(SpeedLimitCallback) = 0;
};

struct MockServiceBus : IServiceBus
{
    void publish(const BreakCommand& cmd) override
    {
        commands_published++;
        last_command = cmd;
    }

    void subscribe(SpeedUpdateCallback callback) override
    {
        speed_update_callback = callback;
    }

    void subscribe(CarDetectedCallback callback) override
    {
        car_detected_callback = callback;
    }

    void subscribe(SpeedLimitCallback callback) override
    {
        speed_limit_callback = callback;
    }

    BreakCommand last_command{};
    int commands_published{};
    SpeedUpdateCallback speed_update_callback{};
    CarDetectedCallback car_detected_callback{};
    SpeedLimitCallback speed_limit_callback{};
};

struct AutoBrake
{
    AutoBrake(IServiceBus& bus) : 
        collision_threshold_s{ 5 },
        speed_mps{},
        speed_limit_mps{ 39 }
    {
        bus.subscribe([this, &bus](const SpeedUpdate& update)
        { 
            speed_mps = update.velocity_mps;
        });

        bus.subscribe([this, &bus](const CarDetected& cd)
        {
            auto relative_velocity_mps = speed_mps - cd.velocity_mps;
            auto time_to_collision_s = cd.distance_m / relative_velocity_mps;
            if(time_to_collision_s > 0 && time_to_collision_s <= collision_threshold_s)
            {
                bus.publish(BreakCommand{ time_to_collision_s });
            }
        });

        bus.subscribe([this, &bus](const SpeedLimitDetected& update)
        { 
            speed_limit_mps = update.speed_mps;
            if(speed_mps > speed_limit_mps)
            {
                bus.publish(BreakCommand{ 0 });
            }
        });
    }

    void set_collision_threshold_s(double x)
    {
        if(x < 1) throw std::runtime_error{ "Collision less than 1." };
        collision_threshold_s = x;
    }

    double get_collision_threshold_s() const
    {
        return collision_threshold_s;
    }

    double get_speed_mps() const
    {
        return speed_mps;
    }

    double get_speed_limit() const
    {
        return speed_limit_mps;
    }

private:
    double collision_threshold_s;
    double speed_mps;
    double speed_limit_mps;
};

TEST_CASE("AutoBrake")
{
    MockServiceBus bus{};
    AutoBrake auto_break{ bus };

    SECTION("initializes speed to zero")
    {
        REQUIRE(auto_break.get_speed_mps() == Approx(0));
    }

    SECTION("initializes sensitivity to five")
    {
        REQUIRE(auto_break.get_collision_threshold_s() == Approx(5));
    }

    SECTION("throws when sensitivity less than one")
    {
        REQUIRE_THROWS(auto_break.set_collision_threshold_s(0.5L));
    }

    SECTION("saves speed after update")
    {
        bus.speed_update_callback(SpeedUpdate{ 100L });
        REQUIRE(100L == auto_break.get_speed_mps());
        bus.speed_update_callback(SpeedUpdate{ 50L });
        REQUIRE(50L == auto_break.get_speed_mps());
        bus.speed_update_callback(SpeedUpdate{ 0L });
        REQUIRE(0L == auto_break.get_speed_mps());
    }

    SECTION("no alert when not imminent")
    {
        auto_break.set_collision_threshold_s(2L);
        bus.speed_update_callback(SpeedUpdate{ 100L });
        bus.car_detected_callback(CarDetected{ 1000L, 50L });
        REQUIRE(bus.commands_published == 0);
    }

    SECTION("alert when imminent")
    {
        auto_break.set_collision_threshold_s(10L);
        bus.speed_update_callback(SpeedUpdate{ 100L });
        bus.car_detected_callback(CarDetected{ 100L, 0L });
        assert(bus.commands_published == 1);
        assert(bus.last_command.time_to_collision_s == Approx(1));
    }

    SECTION("initializes speed limit to 39")
    {
        REQUIRE(auto_break.get_speed_limit() == Approx(39));
    }

    SECTION("saves speed limit after update")
    {
        bus.speed_limit_callback(SpeedLimitDetected{ 25L });
        REQUIRE(25L == auto_break.get_speed_limit());
        bus.speed_limit_callback(SpeedLimitDetected{ 100L });
        REQUIRE(100L == auto_break.get_speed_limit());
        bus.speed_limit_callback(SpeedLimitDetected{ 5L });
        REQUIRE(5L == auto_break.get_speed_limit());
    }

    SECTION("no braking when under speed limit")
    {
        bus.speed_update_callback(SpeedUpdate{ 34L });
        bus.speed_limit_callback(SpeedLimitDetected{ 35L });
        REQUIRE(bus.commands_published == 0);
    }

    SECTION("brake when over speed limit")
    {
        bus.speed_update_callback(SpeedUpdate{ 40L });
        bus.speed_limit_callback(SpeedLimitDetected{ 35L });
        REQUIRE(bus.commands_published == 1);
        REQUIRE(bus.last_command.time_to_collision_s == 0);
    }

    SECTION("brake when speed limit changes")
    {
        bus.speed_update_callback(SpeedUpdate{ 35L });
        bus.speed_limit_callback(SpeedLimitDetected{ 35L });
        bus.speed_update_callback(SpeedUpdate{ 30L });
        bus.speed_limit_callback(SpeedLimitDetected{ 25L });
        REQUIRE(bus.commands_published == 1);
        REQUIRE(bus.last_command.time_to_collision_s == 0);
    }
}