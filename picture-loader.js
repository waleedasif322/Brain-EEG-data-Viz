var app = angular.module('data-viz-proj', ['ui.bootstrap', 'iso.directives']);

app.controller('MainCtrl', function($scope) {

  $scope.images_order = [136, 189, 3, 158, 87, 90, 54, 9, 10, 171, 129, 11, 103, 141, 216, 43, 156, 39, 2, 166, 58, 92, 67, 78, 26, 173, 101, 175, 32, 102, 208, 138, 48, 139, 18, 199, 167, 19, 40, 192, 120, 106, 127, 13, 165, 211, 95, 118, 57, 169, 203, 111, 112, 200, 96, 114, 35, 198, 182, 145, 152, 22, 168, 5, 178, 150, 162, 47, 122, 31, 181, 213, 204, 98, 44, 89, 135, 159, 66, 29, 8, 7, 187, 1, 121, 79, 23, 52, 212, 17, 176, 113, 20, 123, 137, 144, 151, 46, 147, 196, 28, 84, 85, 201, 172, 12, 214, 131, 94, 42, 190, 61, 206, 80, 153, 34, 69, 33, 210, 119, 185, 55, 108, 27, 4, 160, 70, 107, 76, 163, 155, 91, 142, 143, 64, 75, 125, 130, 21, 194];

  $scope.images = {}
  $scope.createImages = function(){
    for (var i=1; i<140; i++){
        $scope.images[i] = '/thumbs/' + $scope.images_order[i] + '.jpg'
    }
    console.log($scope.images[1]);
  }

  $scope.showPath = function (imageNumber) {
    return $scope.images[imageNumber];
  }

})
