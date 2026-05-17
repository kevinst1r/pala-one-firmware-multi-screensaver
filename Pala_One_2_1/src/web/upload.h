#ifndef PALA_WEB_UPLOAD_H
#define PALA_WEB_UPLOAD_H

// Mounts /upload (book upload, multipart) and /upload-sleep (custom sleep
// image upload). Both register a stream handler (chunk receiver) and a done
// handler (final response).
void registerUploadRoutes();

#endif  // PALA_WEB_UPLOAD_H
