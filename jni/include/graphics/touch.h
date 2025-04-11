#ifndef TOUCH_H
#define TOUCH_H

#include <time.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <linux/uinput.h>

#include <list>
#include <chrono>
#include <thread>

template <class item>
class Channel
{
private:
    std::list<item> queue;
    std::mutex m;
    std::condition_variable cv;
    bool closed;

public:
    Channel() : closed(false) {}

    void close()
    {
        std::unique_lock<std::mutex> lock(m);
        closed = true;
        cv.notify_all();
    }

    bool is_closed()
    {
        std::unique_lock<std::mutex> lock(m);
        return closed;
    }

    void put(const item &i)
    {
        std::unique_lock<std::mutex> lock(m);
        if (closed)
            throw std::logic_error("put to closed channel");
        queue.push_back(i);
        cv.notify_one();
    }

    bool get(item &out, bool wait = true)
    {
        std::unique_lock<std::mutex> lock(m);
        if (wait)
            cv.wait(lock, [&]()
                    { return closed || !queue.empty(); });
        if (queue.empty())
            return false;
        out = queue.front();
        queue.pop_front();
        return true;
    }
};

namespace TouchInput
{
    struct input_absinfo absX;
    struct input_absinfo absY;
    struct input_absinfo absSlot;
    const char letterBytes[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

    char *randString(int n)
    {
        char *b = (char *)malloc(n + 1);
        if (b == NULL)
        {
            perror("Failed to allocate memory");
            // exit(1);
            return NULL;
        }

        for (int i = 0; i < n; i++)
        {
            b[i] = letterBytes[rand() % (sizeof(letterBytes) - 1)];
        }
        b[n] = '\0';

        return b;
    }

    int randNum(int n)
    {

        return rand() % n;
    }

    int createUInput(int ifd)
    {
        int ufd;
        const char *uinput = "/dev/uinput";

        ioctl(ifd, EVIOCGABS(ABS_MT_SLOT), &absSlot);
        ioctl(ifd, EVIOCGABS(ABS_MT_POSITION_X), &absX);
        ioctl(ifd, EVIOCGABS(ABS_MT_POSITION_Y), &absY);

        ufd = open(uinput, O_WRONLY | O_NONBLOCK);
        if (ufd < 0)
        {
            perror("Unable to open uinput");
        }

        ioctl(ufd, UI_SET_EVBIT, EV_KEY);
        ioctl(ufd, UI_SET_KEYBIT, BTN_TOUCH);

        ioctl(ufd, UI_SET_EVBIT, EV_ABS);
        ioctl(ufd, UI_SET_ABSBIT, ABS_MT_POSITION_X);
        ioctl(ufd, UI_SET_ABSBIT, ABS_MT_POSITION_Y);
        ioctl(ufd, UI_SET_ABSBIT, ABS_MT_TRACKING_ID);
        ioctl(ufd, UI_SET_PROPBIT, INPUT_PROP_DIRECT);

        struct uinput_user_dev uidev;
        memset(&uidev, 0, sizeof(uidev));
        uidev.id.bustype = 0x1C;
        uidev.id.vendor = randNum(0x2000);
        uidev.id.product = randNum(0x2000);
        uidev.id.version = randNum(0x200);

        char *devName = randString(7);
        strncpy(uidev.name, devName, UINPUT_MAX_NAME_SIZE);
        free(devName);

        uidev.absmin[ABS_MT_POSITION_X] = absX.minimum;
        uidev.absmax[ABS_MT_POSITION_X] = absX.maximum;
        uidev.absmin[ABS_MT_POSITION_Y] = absY.minimum;
        uidev.absmax[ABS_MT_POSITION_Y] = absY.maximum;
        uidev.absmin[ABS_MT_TRACKING_ID] = 0;
        uidev.absmax[ABS_MT_TRACKING_ID] = absSlot.maximum;

        write(ufd, &uidev, sizeof(uidev));

        ioctl(ufd, UI_DEV_CREATE);

        ioctl(ifd, EVIOCGRAB, 1);

        return ufd;
    }

    bool HasSpecificAbs(int device_fd, unsigned int key)
    {
        size_t nchar = ABS_MAX / 8 + 1;
        unsigned char bits[nchar];
        ioctl(device_fd, EVIOCGBIT(EV_ABS, sizeof(bits)), &bits);
        return bits[key / 8] & (1 << (key % 8));
    }

    int isCharDevice(const char *path)
    {
        struct stat st;
        if (stat(path, &st) == -1)
        {
            return 0;
        }

        if (S_ISCHR(st.st_mode))
        {
            return 1;
        }

        return 0;
    }

    int getTouchDevice()
    {
        int fd = -1;
        struct dirent *entry;
        const char *input_path = "/dev/input";

        DIR *dir = opendir(input_path);
        if (!dir)
        {
            return -1;
        }

        while ((entry = readdir(dir)))
        {
            if (!strstr(entry->d_name, "event"))
                continue;

            char devname[PATH_MAX];
            snprintf(devname, sizeof(devname), "%s/%s", input_path, entry->d_name);

            if (!isCharDevice(devname))
            {
                continue;
            }

            fd = open(devname, O_RDONLY);
            if (fd < 0)
            {
                continue;
            }

            if (!HasSpecificAbs(fd, ABS_MT_SLOT))
            {
                continue;
            }

            break;
        }

        closedir(dir);

        return fd;
    }

    void writeEvent(int ifd, int type, int code, int value)
    {
        struct input_event inputEvent = {};

        inputEvent.type = type;
        inputEvent.code = code;
        inputEvent.value = value;

        write(ifd, &inputEvent, sizeof(input_event));
    }

    struct TouchContact
    {
        int posX = -1;
        int posY = -1;
        bool enabled = false;
    };

#define FAKE_CONTACT 9
    int touchXMin;
    int touchXMax;
    int touchYMin;
    int touchYMax;
    int maxContacts;
    int displayWidth;
    int displayHeight;
    int displayRotation;
    int touchDeviceFd = -1;
    int uInputTouchFd = -1;
    int swipeInterval = 10;
    int swipeFrameTime = 15;
    bool isBtnDown = false;
    bool touchSend = false;
    bool touchStart = false;
    std::mutex touchSynMtx;
    Channel<bool> touchChannel;
    struct TouchContact *contacts = NULL;

    void eventReaderThread(int fd)
    {
        int currSlot = 0;
        bool hasSyn = false;
        struct input_event evt;

        if (touchChannel.is_closed())
        {
            return;
        }

        while (read(fd, &evt, sizeof(evt)))
        {

            touchSynMtx.lock();

            switch (evt.type)
            {
            case EV_SYN:
                if (evt.code == SYN_REPORT)
                {
                    hasSyn = true;
                }
                break;
            case EV_KEY:
                if (evt.code == BTN_TOUCH)
                {
                }
                break;
            case EV_ABS:
                switch (evt.code)
                {
                case ABS_MT_SLOT:
                    currSlot = evt.value;

                    break;
                case ABS_MT_TRACKING_ID:
                    contacts[currSlot].enabled = evt.value != -1;

                    break;
                case ABS_MT_POSITION_X:
                    contacts[currSlot].posX = evt.value;

                    break;
                case ABS_MT_POSITION_Y:
                    contacts[currSlot].posY = evt.value;

                    break;
                }
                break;
            }

            touchSynMtx.unlock();

            if (hasSyn)
            {
                touchChannel.put(true);
                hasSyn = false;
            }
        }
    }

    void eventWriterThread(int ufd)
    {
        while (!touchChannel.is_closed())
        {
            bool sync;
            while (touchChannel.get(sync) && sync)
            {

                touchSynMtx.lock();

                int nextSlot = 0;

                for (int i = 0; i < maxContacts; i++)
                {
                    if (contacts[i].enabled && contacts[i].posX > 0 && contacts[i].posY > 0)
                    {
                        writeEvent(ufd, EV_ABS, ABS_MT_POSITION_X, contacts[i].posX);
                        writeEvent(ufd, EV_ABS, ABS_MT_POSITION_Y, contacts[i].posY);
                        writeEvent(ufd, EV_ABS, ABS_MT_TRACKING_ID, i);
                        writeEvent(ufd, EV_SYN, SYN_MT_REPORT, 0x0);

                        nextSlot++;
                    }
                }

                if (nextSlot == 0 && isBtnDown)
                {
                    isBtnDown = false;
                    writeEvent(ufd, EV_SYN, SYN_MT_REPORT, 0x0);
                    writeEvent(ufd, EV_KEY, BTN_TOUCH, 0x0);
                }
                else if (nextSlot == 1 && !isBtnDown)
                {
                    isBtnDown = true;
                    writeEvent(ufd, EV_KEY, BTN_TOUCH, 0x1);
                }

                writeEvent(ufd, EV_SYN, SYN_REPORT, 0x0);

                touchSynMtx.unlock();
            }
        }
    }

    void sendTouchMove(int x, int y)
    {
        if (!touchStart)
        {
            return;
        }

        if (!touchSend)
        {
            touchSend = true;
        }

        touchSynMtx.lock();

        displayWidth = width;
        displayHeight = height;
        displayRotation = orientation;

        // Adjust coordinates based on rotation
        switch (displayRotation)
        {
        case 0: // Portrait
            contacts[FAKE_CONTACT].posX = x * touchXMax / displayWidth + touchXMin;
            contacts[FAKE_CONTACT].posY = y * touchYMax / displayHeight + touchYMin;
            contacts[FAKE_CONTACT].enabled = true;
            break;
        case 1: // Landscape (Clockwise)
            contacts[FAKE_CONTACT].posY = x * touchYMax / displayWidth + touchXMin;
            contacts[FAKE_CONTACT].posX = (displayHeight - y) * touchXMax / displayHeight + touchYMin;
            contacts[FAKE_CONTACT].enabled = true;
            break;
        case 2: // Reverse Portrait
            contacts[FAKE_CONTACT].posX = (displayWidth - x) * touchXMax / displayWidth + touchXMin;
            contacts[FAKE_CONTACT].posY = (displayHeight - y) * touchYMax / displayHeight + touchYMin;
            contacts[FAKE_CONTACT].enabled = true;
            break;
        case 3: // Landscape (Counter-Clockwise)
            contacts[FAKE_CONTACT].posY = (displayWidth - x) * touchYMax / displayWidth + touchXMin;
            contacts[FAKE_CONTACT].posX = y * touchXMax / displayHeight + touchYMin;
            contacts[FAKE_CONTACT].enabled = true;
            break;
        default:
            // Unknown rotation, handle as needed
            break;
        }

        contacts[FAKE_CONTACT].enabled = true;
        touchSynMtx.unlock();
        touchChannel.put(true);
    }

    void sendTouchUp()
    {
        if (!touchStart || !touchSend)
        {
            return;
        }

        touchSend = false;

        touchSynMtx.lock();

        contacts[FAKE_CONTACT].posX = -1;
        contacts[FAKE_CONTACT].posY = -1;
        contacts[FAKE_CONTACT].enabled = false;

        touchSynMtx.unlock();

        touchChannel.put(true);
    }

    void touchInputStart()
    {
        if (!touchStart)
        {
            touchDeviceFd = getTouchDevice();

            if (touchDeviceFd < 0)
            {
                perror("Unable to find touch device\n");
                return;
            }
            else
            {
                uInputTouchFd = createUInput(touchDeviceFd);

                if (uInputTouchFd < 0)
                {
                    perror("Unable to create virtual touch device\n");
                    return;
                }

                touchXMin = absX.minimum;
                touchXMax = absX.maximum - absX.minimum + 1;
                touchYMin = absY.minimum;
                touchYMax = absY.maximum - absY.minimum + 1;

                maxContacts = absSlot.maximum + 1;
                contacts = (struct TouchContact *)malloc((maxContacts) * sizeof(struct TouchContact));
                for (int i = 0; i < maxContacts; i++)
                {
                    contacts[i].posX = -1;
                    contacts[i].posY = -1;
                    contacts[i].enabled = false;
                }

                std::thread(eventReaderThread, touchDeviceFd).detach();
                std::thread(eventWriterThread, uInputTouchFd).detach();

                touchStart = true;
            }
        }
    }

    void touchInputStop()
    {
        if (touchStart && touchDeviceFd > -1 && uInputTouchFd > -1)
        {
            touchChannel.close();

            close(touchDeviceFd);
            close(uInputTouchFd);
            free(contacts);

            touchDeviceFd = -1;
            uInputTouchFd = -1;
            contacts = NULL;

            touchStart = false;
        }
    }
}

#endif // TOUCH_H
