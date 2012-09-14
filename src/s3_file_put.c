#include "include/bucket_connection.h"
static void bucket_connection_on_object_put (struct evhttp_request *req, void *ctx)
{   
   // GetObectCallbackData *data = (GetObectCallbackData *) ctx;
    struct evbuffer *inbuf;
    size_t buf_len;

    LOG_debug ("Object put callback");
    
    if (!req) {
        LOG_err ("Failed to get response from server !");
        goto done;
    }

    if (evhttp_request_get_response_code (req) != HTTP_OK) {
        LOG_err ("response code: %d", evhttp_request_get_response_code (req));
        goto done;
    }

done:
    return;
}

typedef struct {
    bucket_connection_put_object_callback put_object_callback;
    gpointer callback_data;
    struct evbuffer *post_buf;
    struct evbuffer_cb_entry *cb_entry;
} PutObjectCallbackData;

static void on_buffer_depleted_func (struct evbuffer *buffer, const struct evbuffer_cb_info *info, void *arg)
{
    PutObjectCallbackData *data = (PutObjectCallbackData *)arg;

    LOG_debug ("=== BUFFER sent : %zd", info->n_added);
    evbuffer_remove_cb_entry (data->post_buf, data->cb_entry);
    data->put_object_callback (data->callback_data);
    g_free (data);
}


gboolean bucket_connection_put_object (gpointer req_p, struct evbuffer *out_buf,
    bucket_connection_put_object_callback put_object_callback, gpointer callback_data)
{
    struct evhttp_request *req = (struct evhttp_request *)req_p;
    PutObjectCallbackData *data;

    LOG_debug ("Putting object buffer: %zd bytes", evbuffer_get_length (out_buf));

    data = g_new0 (PutObjectCallbackData, 1);
    data->put_object_callback = put_object_callback;
    data->callback_data = callback_data;
    
    // get the request output buffer
    data->post_buf = evhttp_request_get_output_buffer (req);
    evbuffer_add_buffer (data->post_buf, out_buf);
    data->cb_entry = evbuffer_add_cb (data->post_buf, on_buffer_depleted_func, data);

    return TRUE;
}


gpointer bucket_connection_put_object_create_req (BucketConnection *con, const gchar *path,
struct evbuffer *out_buf)
{
    S3Bucket *bucket;
    struct evhttp_request *req;
    gchar *resource_path;
    int res;
    gchar *auth_str;
    struct evbuffer *post_buf;
    struct bufferevent *bev;

    LOG_debug ("Creating put handler for object: %s", path);


    return req;
}

