#pragma once

class Monitor {
public:
    virtual void update() = 0;
    virtual void render() const = 0;
    virtual ~Monitor() = default;
};