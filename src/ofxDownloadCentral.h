//
//  ofxDownloadCentral.h
//  emptyExample
//
//  Created by Oriol Ferrer Mesià on 14/03/14.
//
//

#ifndef emptyExample_ofxDownloadCentral_h
#define emptyExample_ofxDownloadCentral_h

#include "ofMain.h"
#include "ofxBatchDownloader.h"
#include "ofxSimpleHttp.h"
#include "ofEvents.h"
#include "ofEventUtils.h"

/*


// WHAT IS THIS?

 Centralised downloads from any number of objects.
 creates a download queue, each object that requests a download list 
 will be notified when its downloads are ready.
 this is meant mainly to download assets on demand, telling its owner 
 when the asset is ready to load.

 // HOW IS THIS MEANT TO BE USED ?

 1 - define a central downloads object
	ofxDownloadCentral dlc;

 2 - implement a notification callback
	void downloadsFinished(ofxBatchDownloaderReport &report){}

 3 - fill in a list of url with assets you need downloaded
	vector<string> urlList;
 
 4 - start the download
 
	dlc.downloadResources( urlList, this, &myClass::downloadsFinished, destinationFolder );

	it will all will be downloaded in a bg thread
	you will be notified from the main thread when they are done
	you will get a report

 */

class ofxDownloadCentral{

	public:

		ofxDownloadCentral();
		~ofxDownloadCentral();

		void update();
		void draw(float x, float y);

		void cancelCurrentDownload();
		void cancelAllDownloads();

		template <typename ArgumentsType, class ListenerClass>
		void downloadResources(	vector<string>urlList,
								ListenerClass  * listener,
								void (ListenerClass::*listenerMethod)(ArgumentsType&),
								string destinationFolder = "ofxDownloadCentral_downloads"
							   ){

			ofxBatchDownloader * d = new ofxBatchDownloader();
			d->setDownloadFolder(destinationFolder);
			d->addResourcesToDownloadList(urlList);
			ofAddListener(d->resourcesDownloadFinished, listener, listenerMethod); //set the notification to hit our original caller
			downloaders.push_back(d);
			if (!busy) startQueue();

		}

	private:

		void startQueue();

		vector<ofxBatchDownloader*>			downloaders;

		bool								busy;
		bool								verbose;
		ofMutex								mutex;

};

#endif

