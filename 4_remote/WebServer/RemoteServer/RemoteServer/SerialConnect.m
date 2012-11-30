//
// SerialConnect.m
// SerialConnect
//
// Created by David Haselberger.
// Copyleft 2008 __SerialConnect__.
//
// SerialConnect.m is part of SerialConnect.
//
// SerialConnect is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// SerialConnect is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with SerialConnect.  If not, see <http://www.gnu.org/licenses/>.
//
//

#import "SerialConnect.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <paths.h>
#include <termios.h>
#include <sysexits.h>
#include <sys/param.h>
#include <sys/select.h>
#include <sys/time.h>
#include <time.h>
#include <AvailabilityMacros.h>

#ifdef __MWERKS__
#define __CF_USE_FRAMEWORK_INCLUDES__
#endif

#include <CoreFoundation/CoreFoundation.h>

#include <IOKit/IOKitLib.h>
#include <IOKit/serial/IOSerialKeys.h>
#if defined(MAC_OS_X_VERSION_10_3) && (MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_3)
#include <IOKit/serial/ioss.h>
#endif
#include <IOKit/IOBSD.h>


#define kATCommandString	"AT\r"

#ifdef LOCAL_ECHO
#define kOKResponseString	"AT\r\r\nOK\r\n"
#else
#define kOKResponseString	"\r\nOK\r\n"
#endif

enum {
    kNumRetries = 3
};


int serialport_init(const char* serialport, int baud)
{
    struct termios toptions;
    int fd;
	
    fd = open(serialport, O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd == -1)  {
        perror("init_serialport: Unable to open port ");
        return -1;
    }
    
    if (tcgetattr(fd, &toptions) < 0) {
        perror("init_serialport: Couldn't get term attributes");
        return -1;
    }
    speed_t brate = baud; 
    switch(baud) {
		case 4800:   brate=B4800;   break;
		case 9600:   brate=B9600;   break;
		case 38400:  brate=B38400;  break;
		case 57600:  brate=B57600;  break;
		case 115200: brate=B115200; break;
    }
    cfsetispeed(&toptions, brate);
    cfsetospeed(&toptions, brate);
	
    toptions.c_cflag &= PARENB;
    toptions.c_cflag &= CSTOPB;
    toptions.c_cflag &= CSIZE;
    toptions.c_cflag |= CS8;
	
    toptions.c_cflag &= CRTSCTS;
	
    toptions.c_cflag |= CREAD | CLOCAL;  
    toptions.c_iflag &= (IXON | IXOFF | IXANY); 
	
    toptions.c_lflag &= (ICANON | ECHO | ECHOE | ISIG); 
    toptions.c_oflag &= OPOST; 
	
    // see: http://unixwiz.net/techtips/termios-vmin-vtime.html
    toptions.c_cc[VMIN]  = 0;
    toptions.c_cc[VTIME] = 20;
    
    if( tcsetattr(fd, TCSANOW, &toptions) < 0) {
        perror("init_serialport: Couldn't set term attributes");
        return -1;
    }
	
    return fd;
}

@implementation SerialConnect

- (id)init
{
	self = [super init];
    if (self) {
		
	}
    return self;
}




- (void)dealloc
{
	[super dealloc];
}

@synthesize deviceNameString;
@synthesize baudRateString;



- (BOOL)activate
{
	
	NSUserDefaults* arduinoUserDefaults = [[NSUserDefaults alloc] init];
	NSDictionary* keyValueDictionary = [arduinoUserDefaults persistentDomainForName:@"com.serialconnect.SerialConnect"];
	
	if ([keyValueDictionary valueForKey:@"deviceNameString"])
	{
		deviceNameString = [keyValueDictionary valueForKey:@"deviceNameString"];
	}else
	{
		deviceNameString = @"/dev/tty.usbserial-A9007LzF";
		
	}
	if ([keyValueDictionary valueForKey:@"baudRateString"])
	{
		baudRateString = [keyValueDictionary valueForKey:@"baudRateString"];
	}else
	{
		baudRateString = [NSString stringWithFormat:@"9600"];
	}
	
	int descriptor = serialport_init([deviceNameString cStringUsingEncoding:NSASCIIStringEncoding], [baudRateString intValue]);
	if (descriptor == -1)
	{
		return NO;
	}
	_fileHandle = [[NSFileHandle alloc] initWithFileDescriptor: descriptor];
	[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(receiveSerialMessage:) name:NSFileHandleReadCompletionNotification object:_fileHandle];
	[_fileHandle readInBackgroundAndNotify];
	
	return YES;
	
}



- (void)receiveSerialMessage:(NSNotification *)notification
{
    NSData* messageData = [[notification userInfo] objectForKey:NSFileHandleNotificationDataItem];
    if ( [messageData length] == 0 ) 
	{
        [_fileHandle readInBackgroundAndNotify];
        return;
    }
	
    NSString* receivedMessage = [[[NSString alloc] initWithData:messageData encoding:NSASCIIStringEncoding] autorelease];
	
	[[NSNotificationCenter defaultCenter] postNotificationName:@"serialMessageReceived" object:receivedMessage];
	[_fileHandle readInBackgroundAndNotify];
	
}

- (void)sendSerialMessage:(NSString *)message
{
	
	NSData *dataObject = [message dataUsingEncoding:NSASCIIStringEncoding];
	[_fileHandle writeData:dataObject];
	
}


@end
