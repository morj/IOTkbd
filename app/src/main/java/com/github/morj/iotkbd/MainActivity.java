/*
    IOTkbd: UDP-based wireless keyboard
    Copyright 2016 Pavel Nikitin

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    In addition, as a special exception, the copyright holders give
    permission to link the code of portions of this program with the
    OpenSSL library under certain conditions as described in each
    individual source file, and distribute linked combinations including
    the two.

    You must obey the GNU General Public License in all respects for all
    of the code used other than OpenSSL. If you modify file(s) with this
    exception, you may extend this exception to your version of the
    file(s), but you are not obligated to do so. If you do not wish to do
    so, delete this exception statement from your version. If you delete
    this exception statement from all source files in the program, then
    also delete it here.
*/

package com.github.morj.iotkbd;

import android.app.Activity;
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.hardware.usb.UsbAccessory;
import android.hardware.usb.UsbDevice;
import android.hardware.usb.UsbDeviceConnection;
import android.hardware.usb.UsbEndpoint;
import android.hardware.usb.UsbInterface;
import android.hardware.usb.UsbManager;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.util.SparseArray;

import java.util.Collection;

import static android.hardware.usb.UsbConstants.USB_CLASS_HID;
import static android.hardware.usb.UsbConstants.USB_CLASS_PER_INTERFACE;
import static android.hardware.usb.UsbConstants.USB_DIR_IN;
import static android.hardware.usb.UsbConstants.USB_ENDPOINT_XFER_INT;

public class MainActivity extends Activity {
    // private static MainActivity m_instance;
    private UsbAccessory accessory;
    private static final String APPNAME = "IOTKBD";
    private static final String ACTION_USB_PERMISSION = "com.android.example.USB_PERMISSION";
    private static final IntentFilter FILTER_USB_ATTACHED = new IntentFilter(UsbManager.ACTION_USB_DEVICE_ATTACHED);
    private static final IntentFilter FILTER_USB_DETACHED = new IntentFilter(UsbManager.ACTION_USB_DEVICE_DETACHED);
    private static final IntentFilter FILTER_USB_PERMISSION = new IntentFilter(ACTION_USB_PERMISSION);

    private PendingIntent mPermissionIntent;
    private UsbManager manager;
    private SparseArray<Integer> connectedDevices;

    public MainActivity() {
        // m_instance = this;

        connectedDevices = new SparseArray<>();
    }

    // Used to load the 'native-lib' library on application startup.
    static {
        String[] nativeLibs = new String[]{
                "z", "protobuf", "protobuf-generated", "mycrypto", "util", "mynetwork", "statesync", "iotkbd-core"
        };

        for (String lib : nativeLibs) {
            try {
                Log.d(APPNAME, "Loading library: " + lib);
                System.loadLibrary(lib);
            } catch (UnsatisfiedLinkError err) {
                Log.e(APPNAME, "Failed to load: " + lib);
                break;
            }
        }
    }
/*

    protected void onCreate2(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        TextView tv = (TextView) findViewById(R.id.sample_text);
        tv.setText(sendSomeUdp());
    }
*/

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native void sendSomeUdp();

    private static native void notifyDeviceAttached(int fd, int endp);

    private static native void notifyDeviceDetached(int fd);

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        // sendSomeUdp();

        manager = (UsbManager) getSystemService(Context.USB_SERVICE);

        // TODO: this is a leak
        registerReceiver(usbManagerBroadcastReceiver, FILTER_USB_ATTACHED);
        registerReceiver(usbManagerBroadcastReceiver, FILTER_USB_DETACHED);
        registerReceiver(usbManagerBroadcastReceiver, FILTER_USB_PERMISSION);

        mPermissionIntent = PendingIntent.getBroadcast(this, 0, new Intent(ACTION_USB_PERMISSION), 0);

        final Handler handler = new Handler();

        handler.postDelayed(new Runnable() {
            @Override
            public void run() {
                checkForDevices();
            }
        }, 1000);
    }

    private final BroadcastReceiver usbManagerBroadcastReceiver = new BroadcastReceiver() {
        public void onReceive(Context context, Intent intent) {
            try {
                String action = intent.getAction();

                Log.d(APPNAME, "INTENT ACTION: " + action);

                if (ACTION_USB_PERMISSION.equals(action)) {
                    Log.d(APPNAME, "onUsbPermission");

                    synchronized (this) {
                        UsbDevice device = intent.getParcelableExtra(UsbManager.EXTRA_DEVICE);

                        if (intent.getBooleanExtra(UsbManager.EXTRA_PERMISSION_GRANTED, false)) {
                            if (device != null) {
                                final UsbEndpoint endp = getInterestingEndpoint(device);
                                final int fd = connectToDevice(device);
                                Log.d(APPNAME, "device file descriptor: " + fd + ", path: " + device.getDeviceName());

                                if (fd != -1) {
                                    new Thread(new Runnable() {
                                        @Override
                                        public void run() {
                                            notifyDeviceAttached(fd, endp == null ? -1 : endp.getEndpointNumber());
                                        }
                                    }, "I/O daemon for " + fd).start();
                                }
                            }
                        } else {
                            Log.d(APPNAME, "permission denied for device " + device);
                        }
                    }
                }

                /*if (UsbManager.ACTION_USB_DEVICE_ATTACHED.equals(action))
                {
                    Log.d(APPNAME, "onDeviceConnected");

                    synchronized(this)
                    {
                        UsbDevice device = intent.getParcelableExtra(UsbManager.EXTRA_DEVICE);

                        if (device != null)
                        {
                            manager.requestPermission(device, mPermissionIntent);
                        }
                    }
                }*/

                if (UsbManager.ACTION_USB_DEVICE_DETACHED.equals(action)) {
                    Log.d(APPNAME, "onDeviceDisconnected");

                    synchronized (this) {
                        UsbDevice device = intent.getParcelableExtra(UsbManager.EXTRA_DEVICE);

                        Integer fd = connectedDevices.get(device.getDeviceId());

                        if (fd == null) {
                            Log.d(APPNAME, "device not remembered: " + device.getDeviceId());
                        } else {
                            Log.d(APPNAME, "device: " + device.getDeviceId() + " disconnected. fd: " + fd);

                            notifyDeviceDetached(fd);

                            connectedDevices.remove(device.getDeviceId());
                        }
                    }
                }
            } catch (Exception e) {
                Log.d(APPNAME, "Exception: " + e);
            }
        }
    };

    @Override
    protected void onDestroy() {
        super.onDestroy();
        unregisterReceiver(usbManagerBroadcastReceiver);
    }

    private int connectToDevice(UsbDevice device) {
        UsbDeviceConnection connection = manager.openDevice(device);

        if (connection == null) {
            Log.d(APPNAME, "can't insert device with id: " + device.getDeviceId());
            return -1;
        } else {
            int fd = connection.getFileDescriptor();

            if (connectedDevices.get(fd) != null) {
                Log.d(APPNAME, "device already connected: " + device.getDeviceId());
                return -1;
            } else {
                // if we make this, kernel driver will be disconnected
                connection.claimInterface(device.getInterface(0), true);

                Log.d(APPNAME, "inserting device with id: " + device.getDeviceId() + " and file descriptor: " + fd);
                connectedDevices.put(device.getDeviceId(), fd);

                return fd;
            }
        }
    }

    private void checkForDevices() {
        Collection<UsbDevice> devices = manager.getDeviceList().values();
        Log.d(APPNAME, "Devices found: " + devices.size());

        for (UsbDevice device : devices) {
            UsbEndpoint endpoint = getInterestingEndpoint(device);
            if (endpoint != null) {
                Log.d(APPNAME, "Found a device: " + device);

                manager.requestPermission(device, mPermissionIntent);
            } else {
                Log.d(APPNAME, "Skip device: " + device);
            }
        }
    }

    private UsbEndpoint getInterestingEndpoint(UsbDevice device) {
        // return device.getVendorId() == 9494 && device.getProductId() == 23;

        if (device.getDeviceClass() == USB_CLASS_PER_INTERFACE) {
            int size = device.getInterfaceCount();
            for (int i = 0; i < size; i++) {
                UsbInterface intf = device.getInterface(i);
                if (intf.getInterfaceClass() == USB_CLASS_HID && intf.getInterfaceProtocol() == 1) {
                    int endpointSize = intf.getEndpointCount();
                    for (int j = 0; j < endpointSize; j++) {
                        UsbEndpoint ep = intf.getEndpoint(j);
                        if (ep.getType() == USB_ENDPOINT_XFER_INT && ep.getDirection() == USB_DIR_IN) {
                            Log.d(APPNAME, "Endpoint type: " + ep.getType());
                            return ep;
                        }
                    }
                }
            }
        }

        return null;
    }
}
