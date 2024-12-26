#include "mongoose.h"

// Declare HTTP port
const char *s_http_port = "8080";  // Or another port you want to use

// Declare options for the HTTP server
static struct mg_serve_http_opts s_http_server_opts;

// Your event handler function
static void ev_handler(struct mg_connection *nc, int ev, void *p) {
    struct http_message *hm = (struct http_message *) p;

    switch (ev) {
        case MG_EV_HTTP_MSG:  // Changed to MG_EV_HTTP_MSG
            if (mg_http_match_uri(hm, "/api")) {
                mg_http_serve_opts(nc, hm, &s_http_server_opts);
            }
            break;
    }
}

int main() {
    struct mg_mgr mgr;
    struct mg_connection *nc;

    // Initialize the event manager (no second argument needed)
    mg_mgr_init(&mgr);

    // Set the document root for serving static files
    s_http_server_opts.document_root = ".";  // You can adjust this path as needed

    // Listen for HTTP requests
    nc = mg_http_listen(&mgr, s_http_port, ev_handler, &mgr);
    if (nc == NULL) {
        printf("Error starting server on port %s\n", s_http_port);
        return 1;
    }

    // Run the event loop
    printf("Starting HTTP server on port %s\n", s_http_port);
    for (;;) {
        mg_mgr_poll(&mgr, 1000); // Polling for events
    }

    // Free resources before exiting
    mg_mgr_free(&mgr);
    return 0;
}
