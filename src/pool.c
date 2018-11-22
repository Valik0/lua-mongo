/*
** Copyright (C) 2016-2018 Arseny Vakhrushev <arseny.vakhrushev@gmail.com>
**
** Permission is hereby granted, free of charge, to any person obtaining a copy
** of this software and associated documentation files (the "Software"), to deal
** in the Software without restriction, including without limitation the rights
** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
** copies of the Software, and to permit persons to whom the Software is
** furnished to do so, subject to the following conditions:
**
** The above copyright notice and this permission notice shall be included in
** all copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
** OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
** THE SOFTWARE.
*/

#include "common.h"

static int m_popClient(lua_State *L) {
	mongoc_client_pool_t *pool = checkPool(L, 1);
	pushClientFromPool(L, mongoc_client_pool_pop(pool), 1);
	return 1;
}

static int m_pushClient(lua_State *L) {
	mongoc_client_pool_t *pool = checkPool(L, 1);
	mongoc_client_t *client = checkClient(L, 2);
	mongoc_client_pool_push(pool, client);
	return 0;
}

static int m__poolGC(lua_State *L) {
	mongoc_client_pool_destroy(checkPool(L, 1));
//TODO: fix this
//	mongoc_uri_destroy(checkPool(L, 2));
	unsetType(L);
	return 0;
}

static const luaL_Reg poolFuncs[] = {
	{"popClient", m_popClient},
	{"pushClient", m_pushClient},
	{"__gc", m__poolGC},
	{0, 0}
};

int newPool(lua_State *L) {
	mongoc_uri_t *uri;
	uri = mongoc_uri_new(luaL_checkstring(L, 1));
	mongoc_client_pool_t *pool = mongoc_client_pool_new(uri);
	mongoc_client_pool_set_error_api (pool, 2);
	luaL_argcheck(L, pool, 1, "invalid format");
	pushHandle(L, pool, 0, 0);
	setType(L, TYPE_POOL, poolFuncs);
//TODO: fix this
//	pushHandle(L, uri, 0, 1);
//	setType(L, TYPE_URI, NULL);
	return 1;
}

mongoc_client_pool_t *checkPool(lua_State *L, int idx) {
	return *(mongoc_client_pool_t **)luaL_checkudata(L, idx, TYPE_POOL);
}

mongoc_uri_t *checkUri(lua_State *L, int idx) {
	return *(mongoc_uri_t **)luaL_checkudata(L, idx, TYPE_URI);
}
