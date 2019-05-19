#ifndef PERMISSION_H
#define PERMISSION_H

class Permission
{
    public:
        int priority;
        int direction = -1;
        bool canGo = true;

        Permission(int priority);
};
#endif
