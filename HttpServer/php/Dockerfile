# Dockerfile: File di configurazione di Docker del container iot_http
# Programma scritto durante lo svolgimento del progetto finale per il 
# corso di Multimedia App and Internet of Things dell'Universita' degl Studi di Genova

# Copyright (C) 2022  Andrea Valenzano

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

FROM php:apache
RUN docker-php-ext-install mysqli
RUN docker-php-ext-enable mysqli
EXPOSE 80